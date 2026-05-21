//
// Created by jan-hinsch on 5/21/26.
//

#include "computations.hpp"
#include "d2q9.hpp"

void compute_density(const DistributionField& f,  ScalarField rho, int nx,
    int ny)
{
    Kokkos::parallel_for("density_parralel_for",
        Kokkos::MDRangePolicy<Kokkos::Rank<2>>({0, 0}, {nx, ny}),
    [=](const int i, const int j)
    {
        double rho_atpoint = 0.0;

        for (int q = 0; q < Q; q++) {
            rho_atpoint += f(q, i, j);
        }

        rho(i, j) = rho_atpoint;
    });
}

// TODO: i have an if here in kernel maybe find a better way? maybe an epsilon?
void compute_velocity(const DistributionField& f, const ScalarField& rho,
    ScalarField ux, ScalarField uy, int nx, int ny)
{
    Kokkos::parallel_for("density_parralel_for",
        Kokkos::MDRangePolicy<Kokkos::Rank<2>>({0, 0}, {nx, ny}),
[=](const int i, const int j)
    {
        double ux_atpoint = 0.0;
        double uy_atpoint = 0.0;

        for (int q = 0; q < Q; q++) {
            ux_atpoint += f(q, i, j) * cx[q];
            uy_atpoint += f(q, i, j) * cy[q];
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
void streaming(const DistributionField& f, DistributionField f_next, int nx,
    int ny)
{
    Kokkos::parallel_for("streaming",
        Kokkos::MDRangePolicy<Kokkos::Rank<3>>({0, 0, 0},
            {Q, nx, ny}),
[=](int q, int x, int y)
    {
        int ind_x = (x - cx[q] + nx) % nx;

        int ind_y = (y - cy[q] + ny) % ny;

        f_next(q, x, y) = f(q, ind_x, ind_y);
    });
}