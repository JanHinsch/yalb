//
// Created by jan-hinsch on 5/21/26.
//

#pragma once
#include "fields.hpp"

void output_scalar_field_csv(const ScalarField& sf,
    const std::string& filename, int nx, int ny);

void output_velocity_field_csv(const ScalarField& ux, const ScalarField& uy,
    const std::string& filename, int nx, int ny);

void output_scalar_field_binary(const ScalarField& sf,
    const std::string& filename, int nx, int ny);

void output_velocity_field_binary( const ScalarField& ux, const ScalarField& uy,
    const std::string& filename, int nx, int ny);