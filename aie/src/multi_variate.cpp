#include <stdint.h>
#include <stdio.h>
#include <adf.h>
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "kernels.hpp"

#define A_1 -39.696830286653757
#define A_2 220.94609842452050
#define A_3 -275.92851044696869
#define A_4 138.35775186726900
#define A_5 -30.664798066147160
#define A_6 2.5066282774592392
#define B_1 -54.476098798224058
#define B_2 161.58583685804089
#define B_3 -155.69897985988661
#define B_4 66.801311887719720
#define B_5 -13.280681552885721

inline aie::vector<float, 8> cumil_MV(aie::vector<float, 8> input)
{
    aie::vector<float, 8> z = aie::sub(input, (float)0.5);

    aie::vector<float, 8> r = aie::mul(z, z);

    aie::vector<float, 8> res0 = aie::mul((float)A_1, r);
    res0 = aie::add((float)A_2, res0);
    res0 = aie::mul(res0, r);
    res0 = aie::add((float)A_3, res0);
    res0 = aie::mul(res0, r);
    res0 = aie::add((float)A_4, res0);
    res0 = aie::mul(res0, r);
    res0 = aie::add((float)A_5, res0);
    res0 = aie::mul(res0, r);
    res0 = aie::add((float)A_6, res0);
    res0 = aie::mul(res0, z);

    aie::vector<float, 8> res1 = aie::mul((float)B_1, r);
    res1 = aie::add((float)B_2, res1);
    res1 = aie::mul(res1, r);
    res1 = aie::add((float)B_3, res1);
    res1 = aie::mul(res1, r);
    res1 = aie::add((float)B_4, res1);
    res1 = aie::mul(res1, r);
    res1 = aie::add((float)B_5, res1);
    res1 = aie::mul(res1, r);
    res1 = aie::add((float)1.0, res1);
    res1 = aie::inv(res1);

    aie::vector<float, 8> res = aie::mul(res0, res1);
    return res;
}

void inverseCumulativeNormal(input_stream<float> *input_unif_rdn, input_window<float> *corr_matrix, input_window<float> *other_params, output_stream<float> *output_state)
{
    auto corr_matrix_ptr = aie::cbegin_vector_circular<8, 64>((float *)corr_matrix->ptr); // TODO: extend to more than but now it's 8x8

    // other_params_collection[0] = r,  other_params_collection[1] = log_K,  other_params_collection[2] = maturity_time, other_params_collection[3] = (float) steps
    // other_params_collection[4] = (float) paths
    auto other_params_ptr = (float *)other_params->ptr;                               // TODO: extend to more than but now it's 8x8
    aie::vector<float, 8> other_params_collection = aie::load_v<8>(other_params_ptr); // TODO: extend to more than but now it's 8x8
    aie::vector<float, 8> sigma = aie::load_v<8>(other_params_ptr + 8);               // TODO: extend to more than but now it's 8x8
    aie::vector<float, 8> log_initial_price = aie::load_v<8>(other_params_ptr + 16);  // TODO: extend to more than but now it's 8x8
    float r = other_params_collection[0];
    float log_K = other_params_collection[1];
    float maturity_time = other_params_collection[2];
    int32 steps = aie::to_fixed((float)other_params_collection[3]);
    int32 paths = aie::to_fixed((float)other_params_collection[4]);

    for (auto path = 0; path < paths; path++)
    {
        for (int step = 0; step < steps; step++)
            chess_prepare_for_pipelining
            {
                aie::vector<float, 4> input_first_half = readincr_v<4>(input_unif_rdn);
                aie::vector<float, 4> input_second_half = readincr_v<4>(input_unif_rdn);
                aie::vector<float, 8> uniform_number = aie::concat(input_first_half, input_second_half);

                aie::vector<float, 8> gaussian_number = cumil_MV(uniform_number);

                aie::vector<float, 8> correlated_rns;
                for (int asset = 0; asset < 8; asset++) // TODO: extend to more than but now it's 8x8
                {
                    aie::vector<float, 8> row = *corr_matrix_ptr++;

                    aie::vector<float, 8> correlated_row = aie::mul(gaussian_number, row);

                    correlated_rns[asset] = aie::reduce_add(correlated_row);
                }
                writeincr(output_state, correlated_rns);
            }
    }
}