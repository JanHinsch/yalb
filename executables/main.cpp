#include "computations.hpp"
#include "d2q9.hpp"
#include "fields.hpp"
#include "output.hpp"
#include <Kokkos_Core.hpp>
#include <filesystem>
#include <iostream>
#include <mpi.h>
#include "output.hpp"

void run_simulation()
{
    int nx = 150;
    int ny = 100;

    DistributionField f("f",nx, ny, Q);
    DistributionField f_next("f_next",nx, ny, Q);

    // TODO: später villeicht on the fly berechnen
    ScalarField rho("rho", nx, ny);
    ScalarField ux("ux",nx, ny);
    ScalarField uy("uy",nx, ny);

    // init everything to 0 (I use (q,x,z) instead of (x,y,q) apparently thats
    // faster) TODO: check this again why this is the case
    // TODO: (also am I using LayoutLeft or LayotRight) i think right for now
    // Kokkos::parallel_for(
    // "initialize",
    // Kokkos::MDRangePolicy<Kokkos::Rank<3>>(
    //     {0,0,0},
    //     {nx,ny, Q}
    // ),
    // [=](int x, int y, int q)
    // {
    //     f(x,y, q) = 0.0;
    // });

    Kokkos::parallel_for(
    "initialize",
    Kokkos::MDRangePolicy<Kokkos::Rank<2>>(
        {0,0},
        {nx,ny}),
    [=](int x, int y)
    {
        double rho_init = 1.0;

        // slightly higher density in center
        if(x == nx/2 && y == ny/2)
        {
            rho_init = 1.3;
        }

        for(int q = 0; q < Q; ++q)
        {
            f(x,y,q) = w[q] * rho_init;
        }
    });


    Kokkos::fence();
    // main algorithm
    for(int step = 0; step < 100; ++step) {

        compute_density(f, rho, nx, ny);
        compute_velocity(f, rho, ux, uy, nx, ny);

        collison(f, rho, ux, uy, omega, nx, ny);

        streaming(f, f_next, nx, ny);

        std::swap(f, f_next);


        // std::string filename = "../../output/velocity_field_step_" + std::to_string(step) + ".csv";
        // output_velocity_field_csv(ux, uy, filename, nx, ny);

        if (step % 10 == 0) {
            std::string filename = "../../output/csv/density_field_step_" + std::to_string(step) + ".csv";
            output_scalar_field_csv(rho, filename, nx, ny);
        }

    }
}

int main(int argc, char *argv[]) {
    int rank = 0, size = 1;

    MPI_Init(&argc, &argv);
    Kokkos::initialize(argc, argv);

    // Retrieve process infos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    ///////////////////////////

    run_simulation();



    // std::cout << "Hello I am rank " << rank << " of " << size << "\n";
    //
    // if (rank == 0)
    //   //hello_world();
    //
    // auto input_path = "./simulation_test_input.txt";
    //
    // if (not std::filesystem::exists(input_path))
    //   std::cerr << "warning: could not find input file " << input_path << "\n";

    Kokkos::finalize();
    MPI_Finalize();

    return 0;
}
