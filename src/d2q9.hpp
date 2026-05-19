//
// Created by jan-hinsch on 5/19/26.
//

#pragma once
#include <array>

namespace lbm {

    constexpr int Q = 9;

    constexpr std::array<int, Q> cx = { 0, 1, 0, -1, 0, 1, -1, -1, 1 };

    constexpr std::array<int, Q> cy = { 0, 0, 1, 0, -1, 1, 1, -1, -1 };


}
