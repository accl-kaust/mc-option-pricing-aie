#pragma once

#include <adf.h>
#include "aie_api/aie_adf.hpp"
#include "aie_api/aie.hpp"

#define N 156                 // Degree of recursion (19937/128)

#define CEILING(x, y) (((x) + (y) - 1) / (y))

void sfmt_next_state(input_window<int32> *input_state, input_window<float> *other_params, output_stream<float> *output_state);
void inverseCumulativeNormal(input_stream<float> *input_state, input_window<float> *corr_matrix, input_window<float> *other_params, output_stream<float> *output_state);
void path_generate_and_price(input_stream<float> *input_correlated_rns, input_window<float> *other_params, output_stream<float> *price);