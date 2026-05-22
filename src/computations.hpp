//
// Created by jan-hinsch on 5/21/26.
//

#pragma once

#include "fields.hpp"

// TODO: it is more efficient to do streaming + collsion in ONE kernel do that later

// TODO: ALSO remove later the rank 3 kernels so no q iter but rather a normal for loop for q for each thread!!!!


void compute_density(const DistributionField& f, // read-only
    ScalarField& rho,
    int nx,
    int ny);

void compute_velocity(const DistributionField& f,
    const ScalarField& rho,
    ScalarField& ux,
    ScalarField& uy,
    int nx,
    int ny);

// I'm using pull here, thus f is read only (math suggests push thou)
void streaming(const DistributionField& f, // read-only,
    DistributionField& f_next,
    int nx,
    int ny);

void collison(DistributionField& f, const ScalarField& rho,
    const ScalarField& ux, const ScalarField& uy, double omega, int nx, int ny);