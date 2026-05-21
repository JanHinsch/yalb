//
// Created by jan-hinsch on 5/21/26.
//

#include "output.hpp"
#include <iostream>
#include <fstream>

// TODO: later for GPU support make a deep copy!!
void output_scalar_field_csv(const ScalarField& sf, const std::string& filename,
    int nx, int ny) {

    Kokkos::fence(); // waits for all kernels

    std::ofstream file(filename);

    file << "x,y,value\n";

    for(int y = 0; y < ny; ++y)
    {
        for(int x = 0; x < nx; ++x)
        {
            file
                << x << ","
                << y << ","
                << sf(x,y)
                << "\n";
        }
    }

    file.close();
}

void output_velocity_field_csv(const ScalarField& ux, const ScalarField& uy,
    const std::string& filename, int nx, int ny) {

    Kokkos::fence(); // waits for all kernels

    std::ofstream file(filename);

    if(!file.is_open())
    {
        std::cerr << "Failed to open "
                  << filename << "\n";

        return;
    }

    file << "x,y,ux,uy\n";

    for(int y = 0; y < ny; ++y)
    {
        for(int x = 0; x < nx; ++x)
        {
            file
                << x << ","
                << y << ","
                << ux(x,y) << ","
                << uy(x,y) << "\n";
        }
    }

    file.close();
}



// TODO: also just for CPU for now
void output_scalar_field_binary(const ScalarField& sf, const std::string& filename,
    int nx, int ny) {

    Kokkos::fence(); // wait

    std::ofstream file(
        filename,
        std::ios::binary
    );

    file.write(
        reinterpret_cast<const char*>(sf.data()),
        nx * ny * sizeof(double)
    );

    file.close();
}

void output_velocity_field_binary( const ScalarField& ux, const ScalarField& uy,
    const std::string& filename, int nx, int ny)
{
    Kokkos::fence();

    std::ofstream file(
        filename,
        std::ios::binary
    );

    for(int y = 0; y < ny; ++y)
    {
        for(int x = 0; x < nx; ++x)
        {
            double values[2] =
            {
                ux(x,y),
                uy(x,y)
            };

            file.write(
                reinterpret_cast<char*>(values),
                2 * sizeof(double)
            );
        }
    }

    file.close();
}