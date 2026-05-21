//
// Created by jan-hinsch on 5/19/26.
//

#pragma once

#include <Kokkos_Core.hpp>

// for pdf
using DistributionField = Kokkos::View<double***>;

// for quantities
using ScalarField = Kokkos::View<double**>;