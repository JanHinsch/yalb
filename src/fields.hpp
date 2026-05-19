//
// Created by jan-hinsch on 5/19/26.
//

#pragma once

#include <Kokkos_Core.hpp>

namespace lbm {

    // for pdf
    using DistributionField = Kokkos::View<double***>;

    // for quantities
    using LatticeField = Kokkos::View<double***>;
}