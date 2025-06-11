#include <stdint.h>
#include <stdio.h>
#include <adf.h>
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "kernels.hpp"

#define LN2 0.69314718056f     // ln(2)
#define INV_LN2 1.44269504089f // 1 / ln(2)

// Polynomial coefficients for approximating e^r
#define C1 1.0f
#define C2 0.5f
#define C3 0.16666666666f
#define C4 0.04166666666f
#define C5 0.00833333333f
#define C6 0.00138888888f

inline float expf_perso(float x)
{
    if (x == 0.0f)
    {
        return 1.0f; // exp(0) = 1
    }

    // Handle very large or small inputs
    if (x > 88.0f)
        return 8.5070592e+37; // Overflow
    if (x < -88.0f)
        return 0.0f; // Underflow

    // Range reduction: e^x = 2^n * e^r
    int32 n = aie::to_fixed(x * INV_LN2 + 0.5f); // n = round(x / ln(2))
    float r = x - aie::to_float(n) * LN2;        // r = x - n * ln(2)

    // Approximation of e^r using a 5th-degree Taylor series
    float r2 = r * r;
    float result = C1 + r + r2 * (C2 + r * (C3 + r2 * (C4 + r * (C5 + r * C6))));

    // Reconstruct: e^x = 2^n * e^r
    aie::vector<int32, 4> two_vec = aie::broadcast<int32, 4>(1);
    aie::vector<int32, 4> two_n = aie::upshift(two_vec, n); // a vector of 2^n (upshift only applies to vectors)
    return result * two_n[0];                               // ldexpf scales by 2^n
}

void path_generate_and_price(input_stream<float> *input_correlated_rns, input_window<float> *other_params, output_stream<float> *price)
{
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

    float K = expf_perso(log_K);

    float dt = maturity_time / steps;
    // float m89f = -8.5070592e+37; // Some sort of (unexact) -inf used so the the exp evaluates to 0 in the pricer

    aie::vector<float, 8> price_v = aie::broadcast<float, 8>(0.0f);
    float price_tmp = 0.0f;

    aie::vector<float, 8> sigsq = aie::mul(sigma, sigma);
    aie::vector<float, 8> sigsqh = aie::mul(0.5f, sigsq);
    aie::vector<float, 8> rsig = aie::sub(r, sigsqh);
    aie::vector<float, 8> rsigdtv = aie::mul(rsig, dt);
    for (auto path = 0; path < paths; path++)
    {
        aie::vector<float, 8> log_price = aie::zeros<float, 8>();
        for (int step = 0; step < steps; step++)
            chess_prepare_for_pipelining
            {
                // counter++;
                aie::vector<float, 8> correlated_rns = readincr_v<8>(input_correlated_rns);
                aie::vector<float, 8> sigcorr = aie::mul(sigma, correlated_rns);
                aie::vector<float, 8> sigcorrdtv = aie::mul(aie::sqrt(dt), sigcorr);
                aie::vector<float, 8> curr_log = aie::add(rsigdtv, sigcorrdtv);
                log_price = aie::add(log_price, curr_log);
            }

        float log_min_s = aie::reduce_min(aie::add(log_initial_price, log_price));
        // price_tmp += expf_perso(log_min_s > log_K ? log_min_s : m89f);
        price_tmp += log_min_s > log_K ? expf_perso(log_min_s) - K : 0;
    }
    price_v[0] = price_tmp;
    writeincr(price, price_v);
}
