//
// Created by jan-hinsch on 5/19/26.
//

#pragma once
#include <array>


constexpr int Q = 9;

constexpr double omega = 1.2;

constexpr std::array<int, Q> cx = { 0, 1, 0, -1, 0, 1, -1, -1, 1 };

constexpr std::array<int, Q> cy = { 0, 0, 1, 0, -1, 1, 1, -1, -1 };

constexpr std::array<double, Q> w =
{
    4.0 / 9.0,

    1.0 / 9.0,
    1.0 / 9.0,
    1.0 / 9.0,
    1.0 / 9.0,

    1.0 / 36.0,
    1.0 / 36.0,
    1.0 / 36.0,
    1.0 / 36.0
};