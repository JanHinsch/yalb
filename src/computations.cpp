//
// Created by jan-hinsch on 5/21/26.
//

#include "computations.hpp"
#include "d2q9.hpp"

void compute_density(const DistributionField& f,  ScalarField& rho, int nx,
    int ny)
{
    Kokkos::parallel_for("density_parralel_for",
        Kokkos::MDRangePolicy<Kokkos::Rank<2>>(
            {0, 0}, {nx, ny}),
            [=](const int i, const int j)
    {
        double rho_atpoint = 0.0;

        for (int q = 0; q < Q; q++) {
            rho_atpoint += f(i, j, q);
        }

        rho(i, j) = rho_atpoint;
    });
}

// TODO: i have an if here in kernel maybe find a better way? maybe an epsilon?
void compute_velocity(const DistributionField& f, const ScalarField& rho,
    ScalarField& ux, ScalarField& uy, int nx, int ny)
{
    Kokkos::parallel_for(
        "density_parralel_for",
        Kokkos::MDRangePolicy<Kokkos::Rank<2>>(
            {0, 0},
            {nx, ny}),
            [=](const int i, const int j)
    {
        double ux_atpoint = 0.0;
        double uy_atpoint = 0.0;

        for (int q = 0; q < Q; q++) {
            ux_atpoint += f(i, j, q) * cx[q];
            uy_atpoint += f(i, j, q) * cy[q];
        }

        if (rho(i, j) > 0.0) {
            ux(i, j) = ux_atpoint / rho(i, j);
            uy(i, j) = uy_atpoint / rho(i, j);
        } else {
            ux(i, j) = 0.0;
            uy(i, j) = 0.0;
        }
    });
}

// I use pull and start counting from lower left corner!!
void streaming(const DistributionField& f, DistributionField& f_next, int nx,
    int ny)
{
    Kokkos::parallel_for(
    "streaming",
    Kokkos::MDRangePolicy<Kokkos::Rank<2>>(
        {0, 0},
        {nx, ny}),
        [=](int x, int y)
    {
        for (int q = 0; q < Q; q++) {
            int ind_x = (x - cx[q] + nx) % nx;

            int ind_y = (y - cy[q] + ny) % ny;

            f_next(x, y, q) = f(ind_x, ind_y, q);
        }
    });
}

void collison(DistributionField& f, const ScalarField& rho,
    const ScalarField& ux, const ScalarField& uy, double omega, int nx, int ny)
{
    Kokkos::parallel_for(
    "collision",
    Kokkos::MDRangePolicy<Kokkos::Rank<2>>(
        {0, 0},
        {nx, ny}),
    [=](const int x, const int y)
    {
        double rho_atpoint = rho(x, y);
        double ux_atpoint = ux(x, y);
        double uy_atpoint = uy(x, y);

        double u2 = ux_atpoint * ux_atpoint + uy_atpoint * uy_atpoint;

        for (int q = 0; q < Q; q++) {
            // dot product
            const double cu =
                cx[q] * ux_atpoint +
                cy[q] * uy_atpoint;

            // equilibrium distribution
            const double f_eq =
                w[q] * rho_atpoint *
                (
                    1.0
                    + 3.0 * cu
                    + 4.5 * cu * cu
                    - 1.5 * u2
                );

            // collison
            f(x,y,q) =
                f(x,y,q)
                +
                omega *
                (
                    f_eq - f(x,y,q)
                );
        }
    });
}