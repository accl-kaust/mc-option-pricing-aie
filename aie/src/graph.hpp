#pragma once

#include <adf.h>
#include "kernels.hpp"

using namespace adf;

enum placement
{
    top_skewed,
    top_upper,
    top_lower,
    up,
    down,
    unset
};
typedef enum placement Placement;

class graph_overlay_CU : public graph
{
private:
    kernel krnl_unf;
    kernel krnl_cuml;
    kernel krnl_path_gen_price;

public:
    input_port in_seeds;
    input_port in_corr_matrix;
    input_port in_params;
    output_port out_result;

    int col = -1;
    Placement place = unset;

    graph_overlay_CU()
    {
        /****************************************************** Boilerplate starts ******************************************************/
        // Creating the kernels
        krnl_unf = kernel::create(sfmt_next_state);
        krnl_cuml = kernel::create(inverseCumulativeNormal);
        krnl_path_gen_price = kernel::create(path_generate_and_price);
        // Sourcing the kernels
        source(krnl_unf) = "rng.cpp";
        source(krnl_cuml) = "multi_variate.cpp";
        source(krnl_path_gen_price) = "pathgenprice.cpp";
        // Specifying the runtime
        runtime<ratio>(krnl_unf) = 1;
        runtime<ratio>(krnl_cuml) = 1;
        runtime<ratio>(krnl_path_gen_price) = 1;
        /******************************************************* Boilerplate ends *******************************************************/
        /******************************************** Internal and exteral connections start ********************************************/
        // Connecting IOs of the CU graph
        connect<window<N * 4 * sizeof(int32)>>(in_seeds, krnl_unf.in[0]);
        connect<window<8 * 8 * sizeof(float)>>(in_corr_matrix, krnl_cuml.in[1]);
        connect<window<24 * sizeof(float)>>(in_params, krnl_unf.in[1]);
        connect<window<24 * sizeof(float)>>(in_params, krnl_cuml.in[2]);
        connect<window<24 * sizeof(float)>>(in_params, krnl_path_gen_price.in[1]);
        connect<stream>(krnl_path_gen_price.out[0], out_result);
        // Connecting inter-blocks streams
        connect<stream>(krnl_unf.out[0], krnl_cuml.in[0]);
        connect<stream>(krnl_cuml.out[0], krnl_path_gen_price.in[0]);
        /********************************************* Internal and exteral connections end *********************************************/
    };

    void init(Placement place_v = down, int col_v = 24)
    {
        place = place_v;
        col = col_v;
        /***************************************************** Floorplanning starts *****************************************************/
        int krnl_unf_col = -1, krnl_unf_row = -1;
        int krnl_cuml_col = -1, krnl_cuml_row = -1;
        int krnl_path_gen_price_col = -1, krnl_path_gen_price_row = -1;
        switch (place)
        {
        case top_skewed:
            krnl_unf_col = col;
            krnl_unf_row = 6;
            krnl_cuml_col = col;
            krnl_cuml_row = 7;
            krnl_path_gen_price_col = col + 1;
            krnl_path_gen_price_row = 7;
            break;
        case top_upper:
            krnl_unf_col = col;
            krnl_unf_row = 7;
            krnl_cuml_col = col + 1;
            krnl_cuml_row = 7;
            krnl_path_gen_price_col = col + 2;
            krnl_path_gen_price_row = 7;
            break;
        case top_lower:
            krnl_unf_col = col;
            krnl_unf_row = 6;
            krnl_cuml_col = col + 1;
            krnl_cuml_row = 6;
            krnl_path_gen_price_col = col + 2;
            krnl_path_gen_price_row = 6;
            break;
        case up:
            krnl_unf_col = col;
            krnl_unf_row = 3;
            krnl_cuml_col = col;
            krnl_cuml_row = 4;
            krnl_path_gen_price_col = col;
            krnl_path_gen_price_row = 5;
            break;
        case down:
            krnl_unf_col = col;
            krnl_unf_row = 0;
            krnl_cuml_col = col;
            krnl_cuml_row = 1;
            krnl_path_gen_price_col = col;
            krnl_path_gen_price_row = 2;
            break;
        }
        /****************************************************** Floorplanning ends ******************************************************/
        /**************************************************** Placement hints start *****************************************************/
        // Placing the kernels
        location<kernel>(krnl_unf) = tile(krnl_unf_col, krnl_unf_row);
        location<kernel>(krnl_cuml) = tile(krnl_cuml_col, krnl_cuml_row);
        location<kernel>(krnl_path_gen_price) = tile(krnl_path_gen_price_col, krnl_path_gen_price_row);
        // Placing all buffers and stack in the memory tile of the same AIE tile that has the running AIE core: krnl_unf
        location<stack>(krnl_unf) = location<kernel>(krnl_unf);
        location<buffer>(krnl_unf.in[0]) = location<kernel>(krnl_unf);
        location<buffer>(krnl_unf.in[1]) = location<kernel>(krnl_unf);
        // Placing all buffers and stack in the memory tile of the same AIE tile that has the running AIE core: krnl_cuml
        location<stack>(krnl_cuml) = location<kernel>(krnl_cuml);
        location<buffer>(krnl_cuml.in[1]) = location<kernel>(krnl_cuml);
        location<buffer>(krnl_cuml.in[2]) = location<kernel>(krnl_cuml);
        // Placing all buffers and stack in the memory tile of the same AIE tile that has the running AIE core: krnl_path_gen_price
        location<stack>(krnl_path_gen_price) = location<kernel>(krnl_path_gen_price);
        location<buffer>(krnl_path_gen_price.in[1]) = location<kernel>(krnl_path_gen_price);
        /****************************************************** Placement hints end *****************************************************/
    }
};

class graph_double_columned_5_CUs : public graph
{
public:
    input_port in_seeds_col0_top_skewed;

    input_port in_seeds_col0_down;
    input_port in_seeds_col0_up;

    input_port in_seeds_col1_down;
    input_port in_seeds_col1_up;

    input_port in_corr_matrix;

    input_port in_params;

    output_port out_result_col0_top_skewed;

    output_port out_result_col0_down;
    output_port out_result_col0_up;

    output_port out_result_col1_down;
    output_port out_result_col1_up;

    graph_overlay_CU graph_col0_top_skewed;

    graph_overlay_CU graph_col0_down;
    graph_overlay_CU graph_col0_up;

    graph_overlay_CU graph_col1_down;
    graph_overlay_CU graph_col1_up;

    graph_double_columned_5_CUs()
    {
        connect<>(in_seeds_col0_top_skewed, graph_col0_top_skewed.in_seeds);
        connect<>(in_seeds_col0_down, graph_col0_down.in_seeds);
        connect<>(in_seeds_col0_up, graph_col0_up.in_seeds);
        connect<>(in_seeds_col1_down, graph_col1_down.in_seeds);
        connect<>(in_seeds_col1_up, graph_col1_up.in_seeds);

        connect<>(graph_col0_top_skewed.out_result, out_result_col0_top_skewed);
        connect<>(graph_col0_down.out_result, out_result_col0_down);
        connect<>(graph_col0_up.out_result, out_result_col0_up);
        connect<>(graph_col1_down.out_result, out_result_col1_down);
        connect<>(graph_col1_up.out_result, out_result_col1_up);

        connect<>(in_corr_matrix, graph_col0_top_skewed.in_corr_matrix);
        connect<>(in_corr_matrix, graph_col0_down.in_corr_matrix);
        connect<>(in_corr_matrix, graph_col0_up.in_corr_matrix);
        connect<>(in_corr_matrix, graph_col1_down.in_corr_matrix);
        connect<>(in_corr_matrix, graph_col1_up.in_corr_matrix);

        connect<>(in_params, graph_col0_top_skewed.in_params);
        connect<>(in_params, graph_col0_down.in_params);
        connect<>(in_params, graph_col0_up.in_params);
        connect<>(in_params, graph_col1_down.in_params);
        connect<>(in_params, graph_col1_up.in_params);
    };
    void init(int col = 24)
    {
        graph_col0_top_skewed.init(top_skewed, col);
        graph_col0_down.init(down, col);
        graph_col0_up.init(up, col);
        graph_col1_down.init(down, col + 1);
        graph_col1_up.init(up, col + 1);
    }
};

class graph_triple_columned_8_CUs : public graph
{
public:
    input_port in_seeds_col0_top_upper;
    input_port in_seeds_col0_top_lower;

    input_port in_seeds_col0_down;
    input_port in_seeds_col0_up;

    input_port in_seeds_col1_down;
    input_port in_seeds_col1_up;

    input_port in_seeds_col2_down;
    input_port in_seeds_col2_up;

    input_port in_corr_matrix;

    input_port in_params;

    output_port out_result_col0_top_upper;
    output_port out_result_col0_top_lower;

    output_port out_result_col0_down;
    output_port out_result_col0_up;

    output_port out_result_col1_down;
    output_port out_result_col1_up;

    output_port out_result_col2_down;
    output_port out_result_col2_up;

    graph_overlay_CU graph_col0_top_upper;
    graph_overlay_CU graph_col0_top_lower;

    graph_overlay_CU graph_col0_down;
    graph_overlay_CU graph_col0_up;

    graph_overlay_CU graph_col1_down;
    graph_overlay_CU graph_col1_up;

    graph_overlay_CU graph_col2_down;
    graph_overlay_CU graph_col2_up;

    graph_triple_columned_8_CUs()
    {
        connect<>(in_seeds_col0_top_upper, graph_col0_top_upper.in_seeds);
        connect<>(in_seeds_col0_top_lower, graph_col0_top_lower.in_seeds);
        connect<>(in_seeds_col0_up, graph_col0_up.in_seeds);
        connect<>(in_seeds_col0_down, graph_col0_down.in_seeds);
        connect<>(in_seeds_col1_up, graph_col1_up.in_seeds);
        connect<>(in_seeds_col1_down, graph_col1_down.in_seeds);
        connect<>(in_seeds_col2_up, graph_col2_up.in_seeds);
        connect<>(in_seeds_col2_down, graph_col2_down.in_seeds);

        connect<>(graph_col0_top_upper.out_result, out_result_col0_top_upper);
        connect<>(graph_col0_top_lower.out_result, out_result_col0_top_lower);
        connect<>(graph_col0_up.out_result, out_result_col0_up);
        connect<>(graph_col0_down.out_result, out_result_col0_down);
        connect<>(graph_col1_up.out_result, out_result_col1_up);
        connect<>(graph_col1_down.out_result, out_result_col1_down);
        connect<>(graph_col2_up.out_result, out_result_col2_up);
        connect<>(graph_col2_down.out_result, out_result_col2_down);

        connect<>(in_corr_matrix, graph_col0_top_upper.in_corr_matrix);
        connect<>(in_corr_matrix, graph_col0_top_lower.in_corr_matrix);
        connect<>(in_corr_matrix, graph_col0_up.in_corr_matrix);
        connect<>(in_corr_matrix, graph_col0_down.in_corr_matrix);
        connect<>(in_corr_matrix, graph_col1_up.in_corr_matrix);
        connect<>(in_corr_matrix, graph_col1_down.in_corr_matrix);
        connect<>(in_corr_matrix, graph_col2_up.in_corr_matrix);
        connect<>(in_corr_matrix, graph_col2_down.in_corr_matrix);

        connect<>(in_params, graph_col0_top_upper.in_params);
        connect<>(in_params, graph_col0_top_lower.in_params);
        connect<>(in_params, graph_col0_up.in_params);
        connect<>(in_params, graph_col0_down.in_params);
        connect<>(in_params, graph_col1_up.in_params);
        connect<>(in_params, graph_col1_down.in_params);
        connect<>(in_params, graph_col2_up.in_params);
        connect<>(in_params, graph_col2_down.in_params);
    };
    void init(int col = 21)
    {
        graph_col0_top_upper.init(top_upper, col);
        graph_col0_top_lower.init(top_lower, col);

        graph_col0_down.init(down, col);
        graph_col0_up.init(up, col);

        graph_col1_down.init(down, col + 1);
        graph_col1_up.init(up, col + 1);

        graph_col2_down.init(down, col + 2);
        graph_col2_up.init(up, col + 2);
    }
};

class graph_overlay : public graph
{
public:
    input_plio in_seeds_col0_top_upper[16];
    input_plio in_seeds_col0_top_lower[16];
    input_plio in_seeds_col_last_top_skewed;

    input_plio in_seeds_col0_down[17];
    input_plio in_seeds_col0_up[17];

    input_plio in_seeds_col1_down[17];
    input_plio in_seeds_col1_up[17];

    input_plio in_seeds_col2_down[16];
    input_plio in_seeds_col2_up[16];

    input_plio in_corr_matrix[17];

    input_plio in_params[17];

    output_plio out_result_col0_top_upper[16];
    output_plio out_result_col0_top_lower[16];
    output_plio out_result_col_last_top_skewed;

    output_plio out_result_col0_down[17];
    output_plio out_result_col0_up[17];

    output_plio out_result_col1_down[17];
    output_plio out_result_col1_up[17];

    output_plio out_result_col2_down[16];
    output_plio out_result_col2_up[16];

    graph_triple_columned_8_CUs graph_8_CUs_3_cols[16];
    graph_double_columned_5_CUs graph_5_CUs_2_cols;

    graph_overlay()
    {
        for (auto cu = 0; cu < 16; cu++)
        {
            graph_8_CUs_3_cols[cu].init(3 * cu);

            in_seeds_col0_top_upper[cu] = input_plio::create("in_seeds_col0_top_upper" + std::to_string(cu), adf::plio_32_bits, "../../data/inp0_col0_top_upper.txt");
            in_seeds_col0_top_lower[cu] = input_plio::create("in_seeds_col0_top_lower" + std::to_string(cu), adf::plio_32_bits, "../../data/inp0_col0_top_lower.txt");
            in_seeds_col0_down[cu] = input_plio::create("in_seeds_col0_down" + std::to_string(cu), adf::plio_32_bits, "../../data/inp0_col0_down.txt");
            in_seeds_col0_up[cu] = input_plio::create("in_seeds_col0_up" + std::to_string(cu), adf::plio_32_bits, "../../data/inp0_col0_up.txt");
            in_seeds_col1_down[cu] = input_plio::create("in_seeds_col1_down" + std::to_string(cu), adf::plio_32_bits, "../../data/inp0_col1_down.txt");
            in_seeds_col1_up[cu] = input_plio::create("in_seeds_col1_up" + std::to_string(cu), adf::plio_32_bits, "../../data/inp0_col1_up.txt");
            in_seeds_col2_down[cu] = input_plio::create("in_seeds_col2_down" + std::to_string(cu), adf::plio_32_bits, "../../data/inp0_col2_down.txt");
            in_seeds_col2_up[cu] = input_plio::create("in_seeds_col2_up" + std::to_string(cu), adf::plio_32_bits, "../../data/inp0_col2_up.txt");

            out_result_col0_top_upper[cu] = output_plio::create("out_result_col0_top_upper" + std::to_string(cu), adf::plio_32_bits, "./data/out0_col0_top_upper_" + std::to_string(cu) + ".txt");
            out_result_col0_top_lower[cu] = output_plio::create("out_result_col0_top_lower" + std::to_string(cu), adf::plio_32_bits, "./data/out0_col0_top_lower_" + std::to_string(cu) + ".txt");
            out_result_col0_down[cu] = output_plio::create("out_result_col0_down" + std::to_string(cu), adf::plio_32_bits, "./data/out0_col0_down_" + std::to_string(cu) + ".txt");
            out_result_col0_up[cu] = output_plio::create("out_result_col0_up" + std::to_string(cu), adf::plio_32_bits, "./data/out0_col0_up_" + std::to_string(cu) + ".txt");
            out_result_col1_down[cu] = output_plio::create("out_result_col1_down" + std::to_string(cu), adf::plio_32_bits, "./data/out0_col1_down_" + std::to_string(cu) + ".txt");
            out_result_col1_up[cu] = output_plio::create("out_result_col1_up" + std::to_string(cu), adf::plio_32_bits, "./data/out0_col1_up_" + std::to_string(cu) + ".txt");
            out_result_col2_down[cu] = output_plio::create("out_result_col2_down" + std::to_string(cu), adf::plio_32_bits, "./data/out0_col2_down_" + std::to_string(cu) + ".txt");
            out_result_col2_up[cu] = output_plio::create("out_result_col2_up" + std::to_string(cu), adf::plio_32_bits, "./data/out0_col2_up_" + std::to_string(cu) + ".txt");

            in_corr_matrix[cu] = input_plio::create("in_corr_matrix" + std::to_string(cu), adf::plio_32_bits, "../../data/in_corr_matrix.txt");
            in_params[cu] = input_plio::create("in_params" + std::to_string(cu), adf::plio_32_bits, "../../data/in_params.txt");

            connect<>(in_seeds_col0_top_upper[cu].out[0], graph_8_CUs_3_cols[cu].in_seeds_col0_top_upper);
            connect<>(in_seeds_col0_top_lower[cu].out[0], graph_8_CUs_3_cols[cu].in_seeds_col0_top_lower);
            connect<>(in_seeds_col0_down[cu].out[0], graph_8_CUs_3_cols[cu].in_seeds_col0_down);
            connect<>(in_seeds_col0_up[cu].out[0], graph_8_CUs_3_cols[cu].in_seeds_col0_up);
            connect<>(in_seeds_col1_down[cu].out[0], graph_8_CUs_3_cols[cu].in_seeds_col1_down);
            connect<>(in_seeds_col1_up[cu].out[0], graph_8_CUs_3_cols[cu].in_seeds_col1_up);
            connect<>(in_seeds_col2_down[cu].out[0], graph_8_CUs_3_cols[cu].in_seeds_col2_down);
            connect<>(in_seeds_col2_up[cu].out[0], graph_8_CUs_3_cols[cu].in_seeds_col2_up);

            connect<>(graph_8_CUs_3_cols[cu].out_result_col0_top_upper, out_result_col0_top_upper[cu].in[0]);
            connect<>(graph_8_CUs_3_cols[cu].out_result_col0_top_lower, out_result_col0_top_lower[cu].in[0]);
            connect<>(graph_8_CUs_3_cols[cu].out_result_col0_down, out_result_col0_down[cu].in[0]);
            connect<>(graph_8_CUs_3_cols[cu].out_result_col0_up, out_result_col0_up[cu].in[0]);
            connect<>(graph_8_CUs_3_cols[cu].out_result_col1_down, out_result_col1_down[cu].in[0]);
            connect<>(graph_8_CUs_3_cols[cu].out_result_col1_up, out_result_col1_up[cu].in[0]);
            connect<>(graph_8_CUs_3_cols[cu].out_result_col2_down, out_result_col2_down[cu].in[0]);
            connect<>(graph_8_CUs_3_cols[cu].out_result_col2_up, out_result_col2_up[cu].in[0]);

            connect<>(in_corr_matrix[cu].out[0], graph_8_CUs_3_cols[cu].in_corr_matrix);

            connect<>(in_params[cu].out[0], graph_8_CUs_3_cols[cu].in_params);
        }

        graph_5_CUs_2_cols.init(3 * 16);

        in_seeds_col_last_top_skewed = input_plio::create("in_seeds_col_last_top_skewed", adf::plio_32_bits, "../../data/inp0_col_last_top_skewed.txt");
        in_seeds_col0_down[16] = input_plio::create("in_seeds_col0_down" + std::to_string(16), adf::plio_32_bits, "../../data/inp0_col0_down.txt");
        in_seeds_col0_up[16] = input_plio::create("in_seeds_col0_up" + std::to_string(16), adf::plio_32_bits, "../../data/inp0_col0_up.txt");
        in_seeds_col1_down[16] = input_plio::create("in_seeds_col1_down" + std::to_string(16), adf::plio_32_bits, "../../data/inp0_col1_down.txt");
        in_seeds_col1_up[16] = input_plio::create("in_seeds_col1_up" + std::to_string(16), adf::plio_32_bits, "../../data/inp0_col1_up.txt");

        out_result_col_last_top_skewed = output_plio::create("out_result_col_last_top_skewed", adf::plio_32_bits, "./data/out0_col_last_top_skewed.txt");
        out_result_col0_down[16] = output_plio::create("out_result_col0_down" + std::to_string(16), adf::plio_32_bits, "./data/out0_col0_down_" + std::to_string(16) + ".txt");
        out_result_col0_up[16] = output_plio::create("out_result_col0_up" + std::to_string(16), adf::plio_32_bits, "./data/out0_col0_up_" + std::to_string(16) + ".txt");
        out_result_col1_down[16] = output_plio::create("out_result_col1_down" + std::to_string(16), adf::plio_32_bits, "./data/out0_col1_down_" + std::to_string(16) + ".txt");
        out_result_col1_up[16] = output_plio::create("out_result_col1_up" + std::to_string(16), adf::plio_32_bits, "./data/out0_col1_up_" + std::to_string(16) + ".txt");

        in_corr_matrix[16] = input_plio::create("in_corr_matrix" + std::to_string(16), adf::plio_32_bits, "../../data/in_corr_matrix.txt");
        in_params[16] = input_plio::create("in_params" + std::to_string(16), adf::plio_32_bits, "../../data/in_params.txt");

        connect<>(in_seeds_col_last_top_skewed.out[0], graph_5_CUs_2_cols.in_seeds_col0_top_skewed);
        connect<>(in_seeds_col0_down[16].out[0], graph_5_CUs_2_cols.in_seeds_col0_down);
        connect<>(in_seeds_col0_up[16].out[0], graph_5_CUs_2_cols.in_seeds_col0_up);
        connect<>(in_seeds_col1_down[16].out[0], graph_5_CUs_2_cols.in_seeds_col1_down);
        connect<>(in_seeds_col1_up[16].out[0], graph_5_CUs_2_cols.in_seeds_col1_up);

        connect<>(graph_5_CUs_2_cols.out_result_col0_top_skewed, out_result_col_last_top_skewed.in[0]);
        connect<>(graph_5_CUs_2_cols.out_result_col0_down, out_result_col0_down[16].in[0]);
        connect<>(graph_5_CUs_2_cols.out_result_col0_up, out_result_col0_up[16].in[0]);
        connect<>(graph_5_CUs_2_cols.out_result_col1_down, out_result_col1_down[16].in[0]);
        connect<>(graph_5_CUs_2_cols.out_result_col1_up, out_result_col1_up[16].in[0]);

        connect<>(in_corr_matrix[16].out[0], graph_5_CUs_2_cols.in_corr_matrix);

        connect<>(in_params[16].out[0], graph_5_CUs_2_cols.in_params);
    };
};
