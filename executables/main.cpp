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
    int nx = 15;
    int ny = 10;

    DistributionField f("f", Q, nx, ny);
    DistributionField f_next("f_next", Q, nx, ny);

    ScalarField rho("rho", nx, ny);
    ScalarField ux("ux",nx, ny);
    ScalarField uy("uy",nx, ny);

    // init everything to 0 (I use (q,x,z) instead of (x,y,q) apparently thats
    // faster) TODO: check this again why this is the case
    // TODO: (also am I using LayoutLeft or LayotRight) i think right for now
    Kokkos::parallel_for(
    "initialize",
    Kokkos::MDRangePolicy<Kokkos::Rank<3>>(
        {0,0,0},
        {Q,nx,ny}
    ),
    [=](int q, int x, int y)
    {
        f(q,x,y) = 0.0;
    });

    // all flow right
    Kokkos::parallel_for(
    "initialize_east_flow",
    Kokkos::MDRangePolicy<Kokkos::Rank<2>>(
        {0,0},
        {nx,ny}
    ),
    [=](int x, int y)
    {
        f(1,x,y) = 1.0;
    });

    // main algorithm
    for(int step = 0; step < 100; ++step) {

        compute_density(f, rho, nx, ny);
        compute_velocity(f, rho, ux, uy, nx, ny);

        streaming(f, f_next, nx, ny);

        std::swap(f, f_next);

        if(step % 50 == 0)
        {
            std::string filename = "../../output/velocity_field_step_" + std::to_string(step) + ".csv";
            output_velocity_field_csv(ux, uy, filename, nx, ny);
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
