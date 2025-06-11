#include <stdint.h>
#include <stdio.h>
#include <adf.h>
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include "kernels.hpp"

inline aie::vector<int32, 4> shl(aie::vector<int32, 4> v0, int shift)
{
    aie::vector<int32, 4> A = aie::upshift(v0, shift);
    aie::vector<int32, 4> Ap = aie::shuffle_down_fill(v0, aie::zeros<int32, 4>(), 1);
    aie::vector<int32, 4> App = aie::downshift(Ap, (32 - shift));
    A = aie::bit_or(A, App);
    return A;
}

inline aie::vector<int32, 4> shr(aie::vector<int32, 4> r1, int shift)
{
    aie::vector<int32, 4> C = aie::downshift(r1, shift);
    aie::vector<int32, 4> Cp = aie::shuffle_up_fill(r1, aie::zeros<int32, 4>(), 1);
    aie::vector<int32, 4> Cpp = aie::upshift(Cp, (32 - shift));
    C = aie::bit_or(C, Cpp);
    return C;
}

#define M 122                 // Middle point (custom value for SFMT)
#define SFMT_MSK1 0xdfffffefU // Mask for vectorized AND operation
#define SFMT_MSK2 0xddfecb7fU // Mask for vectorized AND operation
#define SFMT_MSK3 0xbffaffffU // Mask for vectorized AND operation
#define SFMT_MSK4 0xbffffff6U // Mask for vectorized AND operation
#define SHIFT_L 8             // Left shift amount for 128-bit integers
#define SHIFT_R 11            // Right shift amount for 32-bit integers

void sfmt_next_state(input_window<int32> *input_state, input_window<float> *other_params, output_stream<float> *output_state)
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

    alignas(aie::vector_decl_align) static int32 MASK[4] = {static_cast<int32>(SFMT_MSK1), static_cast<int32>(SFMT_MSK2), static_cast<int32>(SFMT_MSK3), static_cast<int32>(SFMT_MSK4)};
    aie::vector<int32, 4> vMASK = *(reinterpret_cast<aie::vector<int32, 4> *>(MASK));

    auto itr_input = (int32 *)input_state->ptr;
    alignas(aie::vector_decl_align) int32 states[N * 4];

    // HERE IS A VERY VERY VERY IMPORTANT NOTE!!!
    // This circular iterator is used for two reasons:
    // 1 - Initialise the state buffer declared above (lookup comment: _INIT_STATE_BUFF_)
    // 2 - Update its values throughout the iterations (lookup comment: _UPDATE_STATE_BUFF_)
    // However
    // HOWEVER !
    // HOWEVER !!
    // HOWEVER !!
    // HOWEVER !!!
    // HOW !! EV !! ER !!!!!
    // The state buffer, although could be access using this iterator, it's not (lookup comment: _ACCESS_STATE_BUFF_).
    // This is even though the fourth (read) access uses the modulo to mimic what a circular buffer does.
    // The reason is: It's a crazy mess and unreadable and not easily debuggable to use as it initializes r1 and r2 then
    // goes to the i-th iteration then jumps M steps ahead and then gets back for storing. We can do this but this prevents
    // the reusability of this code. SO PLEASE PROCEED WITH CAUTION!!
    auto states_v_c = aie::begin_vector_circular<4, N * 4>(states);

    for (int i = 0; i < N; i++)
        *states_v_c++ = aie::load_v<4>(itr_input + 4 * i); // _INIT_STATE_BUFF_

    aie::vector<int32, 4> r1 = aie::load_v<4>(states + 4 * (N - 2)); // Cached register for faster access // _ACCESS_STATE_BUFF_
    aie::vector<int32, 4> r2 = aie::load_v<4>(states + 4 * (N - 1)); // Cached register for faster access // _ACCESS_STATE_BUFF_

    int32 path_block_bound = CEILING((2 * paths), N);
    int32 last_path_block_bound = ((2 * paths) % N == 0 ? N : (2 * paths) % N); //_LAST_PARTIAL_

    for (int step = 0; step < steps; step++) // CRUCIAL! : CONTRARY TO THE OTHER MODULES, THE STEPS LOOP IS OUTERMOST HERE AS FUNCTIONALLY IT IS INTERCHANGEABLE WITH THE
                                             // PATHS LOOP BUT SHOULD BE KEPT OUT OF THE STATE LOOP (WHICH IS THE INNERMOST HERE)
    {
        for (int path_block = 0; path_block < path_block_bound - 1; path_block++) // HERE YOU SHOULD PAY EXTREME LEVEL OF ATTENTION!! Since path_block_bound is
                                                                                  // the ceil(2*paths/N), then if 2*paths is a multiple of N then ceil(2*paths/N) = 2*paths/N
                                                                                  // otherwise it's ceil(2*paths/N) < 2*paths/N. This means that we're not sure about the last
                                                                                  // iteration whether it will execute fully or not. This is why we stop at the before last
                                                                                  // iteration and strip the last one down (lookup comment: _LAST_PARTIAL_). We tried making
                                                                                  // the bound of the inner most loop dynamic but that was a lot of overhead.
        {
            for (int i = 0; i < N; i++)
                chess_prepare_for_pipelining
                {
                    // counter++;
                    // Load two 128-bit vectors from the state array
                    aie::vector<int32, 4> v0 = aie::load_v<4>(states + 4 * i);             // _ACCESS_STATE_BUFF_
                    aie::vector<int32, 4> vM = aie::load_v<4>(states + 4 * ((i + M) % N)); // _ACCESS_STATE_BUFF_

                    // Perform the SIMD-based recursion

                    // We want to compute wA = (w << 8) ^ w but since the shift ops here are per 32-bits we use do more
                    aie::vector<int32, 4> A = shl(v0, SHIFT_L);
                    A = aie::bit_xor(A, v0);

                    aie::vector<int32, 4> B = aie::downshift(vM, SHIFT_R); // wD = w >> 8 per 32-bits
                    B = aie::bit_and(vMASK, B);

                    // We want to compute wA = (w >> 8) ^ w but since the shift ops here are per 32-bits we use do more
                    aie::vector<int32, 4> C = shr(r1, SHIFT_L);

                    aie::vector<int32, 4> D = aie::upshift(r2, 18); // wD = w << 18 per 32-bits

                    // Update r1 and r2 for the next iteration
                    r1 = r2;
                    r2 = aie::bit_xor(A, aie::bit_xor(B, aie::bit_xor(C, D))); // Final result of SFMT recursion for the next state

                    r2 = aie::bit_and(0x7fffffff, r2); // Since we can't use unsigned int using AIE APIs (and intrinsics. Interesting, right?), we get rid of the 1 in sign bit

                    // Store the result back to the state array
                    *states_v_c++ = r2; // This assumed to start from 0 and to circulate the moment it should. If you chnaged anything make sure it's not broken // _UPDATE_STATE_BUFF_
                    writeincr(output_state, aie::to_float(r2, 31));
                }
        }

        for (int i = 0; i < last_path_block_bound; i++) //_LAST_PARTIAL_
        {
            aie::vector<int32, 4> v0 = aie::load_v<4>(states + 4 * i);             // _ACCESS_STATE_BUFF_
            aie::vector<int32, 4> vM = aie::load_v<4>(states + 4 * ((i + M) % N)); // _ACCESS_STATE_BUFF_
            aie::vector<int32, 4> A = shl(v0, SHIFT_L);
            A = aie::bit_xor(A, v0);
            aie::vector<int32, 4> B = aie::downshift(vM, SHIFT_R);
            B = aie::bit_and(vMASK, B);
            aie::vector<int32, 4> C = shr(r1, SHIFT_L);
            aie::vector<int32, 4> D = aie::upshift(r2, 18);
            r1 = r2;
            r2 = aie::bit_xor(A, aie::bit_xor(B, aie::bit_xor(C, D)));
            r2 = aie::bit_and(0x7fffffff, r2);
            *states_v_c++ = r2; // _UPDATE_STATE_BUFF_
            writeincr(output_state, aie::to_float(r2, 31));
        }
    }
}