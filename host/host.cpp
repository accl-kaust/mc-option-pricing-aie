#include <cstring>
#include <fstream>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <iomanip>
#include <string>
#include <cmath>
#include "xrt.h"
#include "experimental/xrt_kernel.h"
#include <type_traits> // C++0x

template <typename T>
void parser(T *arr, std::string file_name)
{
    std::ifstream file(file_name); // Open the file
    if (!file.is_open())
    {
        std::cerr << "Failed to open the file!" << std::endl;
    }

    std::string line; // To read each line from the file

    while (std::getline(file, line))
    {
        try
        {
            // Convert the line to a float and push it into the vector
            if (std::is_same<T, unsigned int>::value)
            {
                T value = std::stoi(line);
                *arr++ = value;
            }
            else if (std::is_same<T, float>::value)
            {
                T value = std::stof(line);
                *arr++ = value;
            }
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Invalid float value encountered in line: " << line << std::endl;
        }
    }

    file.close(); // Close the file
}

int main(int argc, char **argv)
{

    // Check input arguments
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Open the device" << std::endl;
    auto device = xrt::device(0);
    std::string binaryFile = argv[1];
    std::cout << "Load the xclbin " << binaryFile << std::endl;
    auto uuid = device.load_xclbin(binaryFile);

    std::cout << "Get references to compute units" << std::endl;
    /******************************************************* pl_mm2s_16x *******************************************************/
    auto kernel_in_seeds_col0_top_upper = xrt::kernel(device, uuid, "pl_seed2s_16x:{pl_seed2s_in_seeds_col0_top_upper}");
    auto kernel_in_seeds_col0_top_lower = xrt::kernel(device, uuid, "pl_seed2s_16x:{pl_seed2s_in_seeds_col0_top_lower}");
    auto kernel_in_seeds_col0_down = xrt::kernel(device, uuid, "pl_seed2s_16x:{pl_seed2s_in_seeds_col0_down}");
    auto kernel_in_seeds_col0_up = xrt::kernel(device, uuid, "pl_seed2s_16x:{pl_seed2s_in_seeds_col0_up}");
    auto kernel_in_seeds_col1_down = xrt::kernel(device, uuid, "pl_seed2s_16x:{pl_seed2s_in_seeds_col1_down}");
    auto kernel_in_seeds_col1_up = xrt::kernel(device, uuid, "pl_seed2s_16x:{pl_seed2s_in_seeds_col1_up}");
    auto kernel_in_seeds_col2_down = xrt::kernel(device, uuid, "pl_seed2s_16x:{pl_seed2s_in_seeds_col2_down}");
    auto kernel_in_seeds_col2_up = xrt::kernel(device, uuid, "pl_seed2s_16x:{pl_seed2s_in_seeds_col2_up}");

    auto kernel_in_corr_matrix = xrt::kernel(device, uuid, "pl_mm2s_16x:{pl_mm2s_in_corr_matrix}");
    auto kernel_in_params = xrt::kernel(device, uuid, "pl_mm2s_16x:{pl_mm2s_in_params}");
    /***************************************************************************************************************************/
    /********************************************************* pl_mm2s *********************************************************/
    auto kernel_in_seeds_col_last_top_skewed = xrt::kernel(device, uuid, "pl_seed2s:{pl_seed2s_in_seeds_col_last_top_skewed}");
    auto kernel_in_seeds_col1_up16 = xrt::kernel(device, uuid, "pl_seed2s:{pl_seed2s_in_seeds_col1_up16}");
    auto kernel_in_seeds_col1_down16 = xrt::kernel(device, uuid, "pl_seed2s:{pl_seed2s_in_seeds_col1_down16}");
    auto kernel_in_seeds_col0_up16 = xrt::kernel(device, uuid, "pl_seed2s:{pl_seed2s_in_seeds_col0_up16}");
    auto kernel_in_seeds_col0_down16 = xrt::kernel(device, uuid, "pl_seed2s:{pl_seed2s_in_seeds_col0_down16}");

    auto kernel_in_corr_matrix16 = xrt::kernel(device, uuid, "pl_mm2s:{pl_mm2s_in_corr_matrix16}");
    auto kernel_in_params16 = xrt::kernel(device, uuid, "pl_mm2s:{pl_mm2s_in_params16}");
    /***************************************************************************************************************************/
    /******************************************************* pl_s2mm_16x *******************************************************/
    auto kernel_out_result_col0_top_upper = xrt::kernel(device, uuid, "pl_s2mm_16x:{pl_s2mm_out_result_col0_top_upper}");
    auto kernel_out_result_col0_top_lower = xrt::kernel(device, uuid, "pl_s2mm_16x:{pl_s2mm_out_result_col0_top_lower}");
    auto kernel_out_result_col0_down = xrt::kernel(device, uuid, "pl_s2mm_16x:{pl_s2mm_out_result_col0_down}");
    auto kernel_out_result_col0_up = xrt::kernel(device, uuid, "pl_s2mm_16x:{pl_s2mm_out_result_col0_up}");
    auto kernel_out_result_col1_down = xrt::kernel(device, uuid, "pl_s2mm_16x:{pl_s2mm_out_result_col1_down}");
    auto kernel_out_result_col1_up = xrt::kernel(device, uuid, "pl_s2mm_16x:{pl_s2mm_out_result_col1_up}");
    auto kernel_out_result_col2_down = xrt::kernel(device, uuid, "pl_s2mm_16x:{pl_s2mm_out_result_col2_down}");
    auto kernel_out_result_col2_up = xrt::kernel(device, uuid, "pl_s2mm_16x:{pl_s2mm_out_result_col2_up}");
    /***************************************************************************************************************************/
    /********************************************************* pl_s2mm *********************************************************/
    auto kernel_out_result_col_last_top_skewed = xrt::kernel(device, uuid, "pl_s2mm:{pl_s2mm_out_result_col_last_top_skewed}");
    auto kernel_out_result_col0_down16 = xrt::kernel(device, uuid, "pl_s2mm:{pl_s2mm_out_result_col0_down16}");
    auto kernel_out_result_col0_up16 = xrt::kernel(device, uuid, "pl_s2mm:{pl_s2mm_out_result_col0_up16}");
    auto kernel_out_result_col1_down16 = xrt::kernel(device, uuid, "pl_s2mm:{pl_s2mm_out_result_col1_down16}");
    auto kernel_out_result_col1_up16 = xrt::kernel(device, uuid, "pl_s2mm:{pl_s2mm_out_result_col1_up16}");
    /***************************************************************************************************************************/
    int size_seeds = 624;
    int size_in_corr_matrix = 8 * 8;
    int size_in_params = 24;
    int size_out0 = 8;

    float *in_corr_matrix[16];
    float *in_params[16];

    float *out_result_col0_top_upper[16];
    float *out_result_col0_top_lower[16];
    float *out_result_col0_down[16];
    float *out_result_col0_up[16];
    float *out_result_col1_down[16];
    float *out_result_col1_up[16];
    float *out_result_col2_down[16];
    float *out_result_col2_up[16];

    for (auto cu = 0; cu < 16; cu++)
    {

        in_corr_matrix[cu] = (float *)malloc(size_in_corr_matrix * sizeof(float));
        in_params[cu] = (float *)malloc(size_in_params * sizeof(float));

        out_result_col0_top_upper[cu] = (float *)malloc(size_out0 * sizeof(float));
        out_result_col0_top_lower[cu] = (float *)malloc(size_out0 * sizeof(float));
        out_result_col0_down[cu] = (float *)malloc(size_out0 * sizeof(float));
        out_result_col0_up[cu] = (float *)malloc(size_out0 * sizeof(float));
        out_result_col1_down[cu] = (float *)malloc(size_out0 * sizeof(float));
        out_result_col1_up[cu] = (float *)malloc(size_out0 * sizeof(float));
        out_result_col2_down[cu] = (float *)malloc(size_out0 * sizeof(float));
        out_result_col2_up[cu] = (float *)malloc(size_out0 * sizeof(float));
    }

    float *in_corr_matrix16 = (float *)malloc(size_in_corr_matrix * sizeof(float));
    float *in_params16 = (float *)malloc(size_in_params * sizeof(float));

    float *out_result_col_last_top_skewed = (float *)malloc(size_out0 * sizeof(float));
    float *out_result_col0_down16 = (float *)malloc(size_out0 * sizeof(float));
    float *out_result_col0_up16 = (float *)malloc(size_out0 * sizeof(float));
    float *out_result_col1_down16 = (float *)malloc(size_out0 * sizeof(float));
    float *out_result_col1_up16 = (float *)malloc(size_out0 * sizeof(float));

    std::cout << "Parsing the data" << std::endl;
    for (auto cu = 0; cu < 16; cu++)
    {
        parser<float>(in_corr_matrix[cu], "../aie/data/in_corr_matrix.txt");
        parser<float>(in_params[cu], "../aie/data/in_params.txt");
    }

    parser<float>(in_corr_matrix16, "../aie/data/in_corr_matrix.txt");
    parser<float>(in_params16, "../aie/data/in_params.txt");

    std::cout << "Creating device buffers" << std::endl;

    xrt::bo in_corr_matrix_buff[16];
    xrt::bo in_params_buff[16];

    xrt::bo out_result_col0_top_upper_buff[16];
    xrt::bo out_result_col0_top_lower_buff[16];
    xrt::bo out_result_col0_down_buff[16];
    xrt::bo out_result_col0_up_buff[16];
    xrt::bo out_result_col1_down_buff[16];
    xrt::bo out_result_col1_up_buff[16];
    xrt::bo out_result_col2_down_buff[16];
    xrt::bo out_result_col2_up_buff[16];

    for (auto cu = 0; cu < 16; cu++)
    {
        in_corr_matrix_buff[cu] = xrt::bo(device, size_in_corr_matrix * sizeof(float), kernel_in_corr_matrix.group_id(cu));
        in_params_buff[cu] = xrt::bo(device, size_in_params * sizeof(float), kernel_in_params.group_id(cu));

        out_result_col0_top_upper_buff[cu] = xrt::bo(device, size_out0 * sizeof(float), kernel_out_result_col0_top_upper.group_id(cu));
        out_result_col0_top_lower_buff[cu] = xrt::bo(device, size_out0 * sizeof(float), kernel_out_result_col0_top_lower.group_id(cu));
        out_result_col0_down_buff[cu] = xrt::bo(device, size_out0 * sizeof(float), kernel_out_result_col0_down.group_id(cu));
        out_result_col0_up_buff[cu] = xrt::bo(device, size_out0 * sizeof(float), kernel_out_result_col0_up.group_id(cu));
        out_result_col1_down_buff[cu] = xrt::bo(device, size_out0 * sizeof(float), kernel_out_result_col1_down.group_id(cu));
        out_result_col1_up_buff[cu] = xrt::bo(device, size_out0 * sizeof(float), kernel_out_result_col1_up.group_id(cu));
        out_result_col2_down_buff[cu] = xrt::bo(device, size_out0 * sizeof(float), kernel_out_result_col1_down.group_id(cu));
        out_result_col2_up_buff[cu] = xrt::bo(device, size_out0 * sizeof(float), kernel_out_result_col1_up.group_id(cu));
    }

    xrt::bo in_corr_matrix16_buff = xrt::bo(device, size_in_corr_matrix * sizeof(float), kernel_in_corr_matrix16.group_id(0));
    xrt::bo in_params16_buff = xrt::bo(device, size_in_params * sizeof(float), kernel_in_params16.group_id(0));

    xrt::bo out_result_col_last_top_skewed_buff = xrt::bo(device, size_out0 * sizeof(float), kernel_out_result_col_last_top_skewed.group_id(0));
    xrt::bo out_result_col0_down16_buff = xrt::bo(device, size_out0 * sizeof(float), kernel_out_result_col0_down16.group_id(0));
    xrt::bo out_result_col0_up16_buff = xrt::bo(device, size_out0 * sizeof(float), kernel_out_result_col0_up16.group_id(0));
    xrt::bo out_result_col1_down16_buff = xrt::bo(device, size_out0 * sizeof(float), kernel_out_result_col1_down16.group_id(0));
    xrt::bo out_result_col1_up16_buff = xrt::bo(device, size_out0 * sizeof(float), kernel_out_result_col1_up16.group_id(0));

    std::cout << "Preparing the data on the device buffers" << std::endl;
    for (auto cu = 0; cu < 16; cu++)
    {
        in_corr_matrix_buff[cu].write(in_corr_matrix[cu]);
        in_params_buff[cu].write(in_params[cu]);
    }

    in_corr_matrix16_buff.write(in_corr_matrix16);
    in_params16_buff.write(in_params16);

    std::cout << "Synchornizing the data on the device buffers" << std::endl;
    for (auto cu = 0; cu < 16; cu++)
    {
        in_corr_matrix_buff[cu].sync(XCL_BO_SYNC_BO_TO_DEVICE);
        in_params_buff[cu].sync(XCL_BO_SYNC_BO_TO_DEVICE);
    }

    in_corr_matrix16_buff.sync(XCL_BO_SYNC_BO_TO_DEVICE);
    in_params16_buff.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    std::cout << "Creating the runs" << std::endl;
    xrt::run run_kernel_in_seeds_col0_top_upper(kernel_in_seeds_col0_top_upper);
    xrt::run run_kernel_in_seeds_col0_top_lower(kernel_in_seeds_col0_top_lower);
    xrt::run run_kernel_in_seeds_col0_down(kernel_in_seeds_col0_down);
    xrt::run run_kernel_in_seeds_col0_up(kernel_in_seeds_col0_up);
    xrt::run run_kernel_in_seeds_col1_down(kernel_in_seeds_col1_down);
    xrt::run run_kernel_in_seeds_col1_up(kernel_in_seeds_col1_up);
    xrt::run run_kernel_in_seeds_col2_down(kernel_in_seeds_col2_down);
    xrt::run run_kernel_in_seeds_col2_up(kernel_in_seeds_col2_up);

    xrt::run run_kernel_in_corr_matrix(kernel_in_corr_matrix);
    xrt::run run_kernel_in_params(kernel_in_params);

    xrt::run run_kernel_out_result_col0_top_upper(kernel_out_result_col0_top_upper);
    xrt::run run_kernel_out_result_col0_top_lower(kernel_out_result_col0_top_lower);
    xrt::run run_kernel_out_result_col0_down(kernel_out_result_col0_down);
    xrt::run run_kernel_out_result_col0_up(kernel_out_result_col0_up);
    xrt::run run_kernel_out_result_col1_down(kernel_out_result_col1_down);
    xrt::run run_kernel_out_result_col1_up(kernel_out_result_col1_up);
    xrt::run run_kernel_out_result_col2_down(kernel_out_result_col2_down);
    xrt::run run_kernel_out_result_col2_up(kernel_out_result_col2_up);

    for (auto cu = 0; cu < 16; cu++)
    {
        run_kernel_in_seeds_col0_top_upper.set_arg(cu, 1234);
        run_kernel_in_seeds_col0_top_upper.set_arg(32 + cu, size_seeds);

        run_kernel_in_seeds_col0_top_lower.set_arg(cu, 1235);
        run_kernel_in_seeds_col0_top_lower.set_arg(32 + cu, size_seeds);

        run_kernel_in_seeds_col0_down.set_arg(cu, 1236);
        run_kernel_in_seeds_col0_down.set_arg(32 + cu, size_seeds);

        run_kernel_in_seeds_col0_up.set_arg(cu, 1237);
        run_kernel_in_seeds_col0_up.set_arg(32 + cu, size_seeds);

        run_kernel_in_seeds_col1_down.set_arg(cu, 1238);
        run_kernel_in_seeds_col1_down.set_arg(32 + cu, size_seeds);

        run_kernel_in_seeds_col1_up.set_arg(cu, 1239);
        run_kernel_in_seeds_col1_up.set_arg(32 + cu, size_seeds);

        run_kernel_in_seeds_col2_down.set_arg(cu, 1240);
        run_kernel_in_seeds_col2_down.set_arg(32 + cu, size_seeds);

        run_kernel_in_seeds_col2_up.set_arg(cu, 1241);
        run_kernel_in_seeds_col2_up.set_arg(32 + cu, size_seeds);

        run_kernel_in_corr_matrix.set_arg(cu, in_corr_matrix_buff[cu]);
        run_kernel_in_corr_matrix.set_arg(32 + cu, size_in_corr_matrix);

        run_kernel_in_params.set_arg(cu, in_params_buff[cu]);
        run_kernel_in_params.set_arg(32 + cu, size_in_params);

        run_kernel_out_result_col0_top_upper.set_arg(cu, out_result_col0_top_upper_buff[cu]);
        run_kernel_out_result_col0_top_upper.set_arg(32 + cu, size_out0);

        run_kernel_out_result_col0_top_lower.set_arg(cu, out_result_col0_top_lower_buff[cu]);
        run_kernel_out_result_col0_top_lower.set_arg(32 + cu, size_out0);

        run_kernel_out_result_col0_down.set_arg(cu, out_result_col0_down_buff[cu]);
        run_kernel_out_result_col0_down.set_arg(32 + cu, size_out0);

        run_kernel_out_result_col0_up.set_arg(cu, out_result_col0_up_buff[cu]);
        run_kernel_out_result_col0_up.set_arg(32 + cu, size_out0);

        run_kernel_out_result_col1_down.set_arg(cu, out_result_col1_down_buff[cu]);
        run_kernel_out_result_col1_down.set_arg(32 + cu, size_out0);

        run_kernel_out_result_col1_up.set_arg(cu, out_result_col1_up_buff[cu]);
        run_kernel_out_result_col1_up.set_arg(32 + cu, size_out0);

        run_kernel_out_result_col2_down.set_arg(cu, out_result_col2_down_buff[cu]);
        run_kernel_out_result_col2_down.set_arg(32 + cu, size_out0);

        run_kernel_out_result_col2_up.set_arg(cu, out_result_col2_up_buff[cu]);
        run_kernel_out_result_col2_up.set_arg(32 + cu, size_out0);
    }

    xrt::run run_kernel_in_seeds_col_last_top_skewed(kernel_in_seeds_col_last_top_skewed);
    run_kernel_in_seeds_col_last_top_skewed.set_arg(0, 1242);
    run_kernel_in_seeds_col_last_top_skewed.set_arg(2, size_seeds);
    xrt::run run_kernel_in_seeds_col1_up16(kernel_in_seeds_col1_up16);
    run_kernel_in_seeds_col1_up16.set_arg(0, 1243);
    run_kernel_in_seeds_col1_up16.set_arg(2, size_seeds);
    xrt::run run_kernel_in_seeds_col1_down16(kernel_in_seeds_col1_down16);
    run_kernel_in_seeds_col1_down16.set_arg(0, 1244);
    run_kernel_in_seeds_col1_down16.set_arg(2, size_seeds);
    xrt::run run_kernel_in_seeds_col0_up16(kernel_in_seeds_col0_up16);
    run_kernel_in_seeds_col0_up16.set_arg(0, 1245);
    run_kernel_in_seeds_col0_up16.set_arg(2, size_seeds);
    xrt::run run_kernel_in_seeds_col0_down16(kernel_in_seeds_col0_down16);
    run_kernel_in_seeds_col0_down16.set_arg(0, 1246);
    run_kernel_in_seeds_col0_down16.set_arg(2, size_seeds);

    xrt::run run_kernel_in_corr_matrix16(kernel_in_corr_matrix16);
    run_kernel_in_corr_matrix16.set_arg(0, in_corr_matrix16_buff);
    run_kernel_in_corr_matrix16.set_arg(2, size_in_corr_matrix);
    xrt::run run_kernel_in_params16(kernel_in_params16);
    run_kernel_in_params16.set_arg(0, in_params16_buff);
    run_kernel_in_params16.set_arg(2, size_in_params);

    xrt::run run_kernel_out_result_col_last_top_skewed(kernel_out_result_col_last_top_skewed);
    run_kernel_out_result_col_last_top_skewed.set_arg(0, out_result_col_last_top_skewed_buff);
    run_kernel_out_result_col_last_top_skewed.set_arg(2, size_out0);
    xrt::run run_kernel_out_result_col0_down16(kernel_out_result_col0_down16);
    run_kernel_out_result_col0_down16.set_arg(0, out_result_col0_down16_buff);
    run_kernel_out_result_col0_down16.set_arg(2, size_out0);
    xrt::run run_kernel_out_result_col0_up16(kernel_out_result_col0_up16);
    run_kernel_out_result_col0_up16.set_arg(0, out_result_col0_up16_buff);
    run_kernel_out_result_col0_up16.set_arg(2, size_out0);
    xrt::run run_kernel_out_result_col1_down16(kernel_out_result_col1_down16);
    run_kernel_out_result_col1_down16.set_arg(0, out_result_col1_down16_buff);
    run_kernel_out_result_col1_down16.set_arg(2, size_out0);
    xrt::run run_kernel_out_result_col1_up16(kernel_out_result_col1_up16);
    run_kernel_out_result_col1_up16.set_arg(0, out_result_col1_up16_buff);
    run_kernel_out_result_col1_up16.set_arg(2, size_out0);
    /********************** */

    std::cout << "Launching" << std::endl;
    std::chrono::duration<double> fpga_duration = std::chrono::nanoseconds::zero();
    auto fpga_begin = std::chrono::high_resolution_clock::now();

    run_kernel_in_seeds_col0_top_upper.start();
    run_kernel_in_seeds_col0_top_lower.start();
    run_kernel_in_seeds_col0_down.start();
    run_kernel_in_seeds_col0_up.start();
    run_kernel_in_seeds_col1_down.start();
    run_kernel_in_seeds_col1_up.start();
    run_kernel_in_seeds_col2_down.start();
    run_kernel_in_seeds_col2_up.start();

    run_kernel_in_corr_matrix.start();
    run_kernel_in_params.start();

    run_kernel_in_seeds_col_last_top_skewed.start();
    run_kernel_in_seeds_col1_up16.start();
    run_kernel_in_seeds_col1_down16.start();
    run_kernel_in_seeds_col0_up16.start();
    run_kernel_in_seeds_col0_down16.start();

    run_kernel_in_corr_matrix16.start();
    run_kernel_in_params16.start();
    // std::cout << "Input kernels started" << std::endl;
    run_kernel_out_result_col0_top_upper.start();
    run_kernel_out_result_col0_top_lower.start();
    run_kernel_out_result_col0_down.start();
    run_kernel_out_result_col0_up.start();
    run_kernel_out_result_col1_down.start();
    run_kernel_out_result_col1_up.start();
    run_kernel_out_result_col2_down.start();
    run_kernel_out_result_col2_up.start();

    run_kernel_out_result_col_last_top_skewed.start();
    run_kernel_out_result_col0_down16.start();
    run_kernel_out_result_col0_up16.start();
    run_kernel_out_result_col1_down16.start();
    run_kernel_out_result_col1_up16.start();
    // std::cout << "Output kernel started" << std::endl;

    run_kernel_in_seeds_col0_top_upper.wait();
    // std::cout << "run_kernel_in_seeds_col0_top_upper completed" << std::endl;
    run_kernel_in_seeds_col0_top_lower.wait();
    // std::cout << "run_kernel_in_seeds_col0_top_lower completed" << std::endl;
    run_kernel_in_seeds_col0_down.wait();
    // std::cout << "run_kernel_in_seeds_col0_down completed" << std::endl;
    run_kernel_in_seeds_col0_up.wait();
    // std::cout << "run_kernel_in_seeds_col0_up completed" << std::endl;
    run_kernel_in_seeds_col1_down.wait();
    // std::cout << "run_kernel_in_seeds_col1_down completed" << std::endl;
    run_kernel_in_seeds_col1_up.wait();
    // std::cout << "run_kernel_in_seeds_col1_up completed" << std::endl;
    run_kernel_in_seeds_col2_down.wait();
    // std::cout << "run_kernel_in_seeds_col2_down completed" << std::endl;
    run_kernel_in_seeds_col2_up.wait();
    // std::cout << "run_kernel_in_seeds_col2_up completed" << std::endl;

    run_kernel_in_corr_matrix.wait();
    // std::cout << "run_kernel_in_corr_matrix completed" << std::endl;
    run_kernel_in_params.wait();
    // // std::cout << "run_kernel_in_params completed" << std::endl;

    run_kernel_in_seeds_col_last_top_skewed.wait();
    // std::cout << "run_kernel_in_seeds_col_last_top_skewed completed" << std::endl;
    run_kernel_in_seeds_col1_up16.wait();
    // std::cout << "run_kernel_in_seeds_col1_up16 completed" << std::endl;
    run_kernel_in_seeds_col1_down16.wait();
    // std::cout << "run_kernel_in_seeds_col1_down16. completed" << std::endl;
    run_kernel_in_seeds_col0_up16.wait();
    // std::cout << "run_kernel_in_seeds_col0_up16 completed" << std::endl;
    run_kernel_in_seeds_col0_down16.wait();
    // std::cout << "run_kernel_in_seeds_col0_down16 completed" << std::endl;

    run_kernel_in_corr_matrix16.wait();
    // std::cout << "run_kernel_in_corr_matrix16 completed" << std::endl;
    run_kernel_in_params16.wait();
    // std::cout << "run_kernel_in_params16 completed" << std::endl;
    // std::cout << "Input kernels completed" << std::endl;
    run_kernel_out_result_col0_top_upper.wait();
    run_kernel_out_result_col0_top_lower.wait();
    run_kernel_out_result_col0_down.wait();
    run_kernel_out_result_col0_up.wait();
    run_kernel_out_result_col1_down.wait();
    run_kernel_out_result_col1_up.wait();
    run_kernel_out_result_col2_down.wait();
    run_kernel_out_result_col2_up.wait();

    run_kernel_out_result_col_last_top_skewed.wait();
    run_kernel_out_result_col0_down16.wait();
    run_kernel_out_result_col0_up16.wait();
    run_kernel_out_result_col1_down16.wait();
    run_kernel_out_result_col1_up16.wait();
    // std::cout << "Output kernel completed" << std::endl;
    auto fpga_end = std::chrono::high_resolution_clock::now();
    fpga_duration = fpga_end - fpga_begin;
    // Synchronize the output buffer data from the device
    std::cout << "Done! Synchronize the output buffer data from the device" << std::endl;

    for (auto cu = 0; cu < 16; cu++)
    {
        out_result_col0_top_upper_buff[cu].sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        out_result_col0_top_lower_buff[cu].sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        out_result_col0_down_buff[cu].sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        out_result_col0_up_buff[cu].sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        out_result_col1_down_buff[cu].sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        out_result_col1_up_buff[cu].sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        out_result_col2_down_buff[cu].sync(XCL_BO_SYNC_BO_FROM_DEVICE);
        out_result_col2_up_buff[cu].sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    }

    out_result_col_last_top_skewed_buff.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    out_result_col0_down16_buff.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    out_result_col0_up16_buff.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    out_result_col1_down16_buff.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    out_result_col1_up16_buff.sync(XCL_BO_SYNC_BO_FROM_DEVICE);

    std::cout << "Reading the data" << std::endl;
    for (auto cu = 0; cu < 16; cu++)
    {
        out_result_col0_top_upper_buff[cu].read(out_result_col0_top_upper[cu]);
        out_result_col0_top_lower_buff[cu].read(out_result_col0_top_lower[cu]);
        out_result_col0_down_buff[cu].read(out_result_col0_down[cu]);
        out_result_col0_up_buff[cu].read(out_result_col0_up[cu]);
        out_result_col1_down_buff[cu].read(out_result_col1_down[cu]);
        out_result_col1_up_buff[cu].read(out_result_col1_up[cu]);
        out_result_col2_down_buff[cu].read(out_result_col2_down[cu]);
        out_result_col2_up_buff[cu].read(out_result_col2_up[cu]);
    }

    out_result_col_last_top_skewed_buff.read(out_result_col_last_top_skewed);
    out_result_col0_down16_buff.read(out_result_col0_down16);
    out_result_col0_up16_buff.read(out_result_col0_up16);
    out_result_col1_down16_buff.read(out_result_col1_down16);
    out_result_col1_up16_buff.read(out_result_col1_up16);

    std::cout << "Exec time on cold : " << fpga_duration.count() << " s " << std::endl;
    // std::cout << " Prices are :" << std::endl;
    float price = 0.0f;
    for (auto cu = 0; cu < 16; cu++)
    {
        // std::cout << "out_result_col0_top_upper[" << cu << "] = " << out_result_col0_top_upper[cu][0] << std::endl;
        // std::cout << "out_result_col0_top_lower[" << cu << "] = " << out_result_col0_top_lower[cu][0] << std::endl;
        // std::cout << "out_result_col0_down[" << cu << "] = " << out_result_col0_down[cu][0] << std::endl;
        // std::cout << "out_result_col0_up[" << cu << "] = " << out_result_col0_up[cu][0] << std::endl;
        // std::cout << "out_result_col1_down[" << cu << "] = " << out_result_col1_down[cu][0] << std::endl;
        // std::cout << "out_result_col1_up[" << cu << "] = " << out_result_col1_up[cu][0] << std::endl;
        // std::cout << "out_result_col2_down[" << cu << "] = " << out_result_col2_down[cu][0] << std::endl;
        // std::cout << "out_result_col2_up[" << cu << "] = " << out_result_col2_up[cu][0] << std::endl;
        price += out_result_col0_top_upper[cu][0] + out_result_col0_top_lower[cu][0] + out_result_col0_down[cu][0] + out_result_col0_up[cu][0] + out_result_col1_down[cu][0] + out_result_col1_up[cu][0] + out_result_col2_down[cu][0] + out_result_col2_up[cu][0];
    }

    // std::cout << "out_result_col_last_top_skewed = " << out_result_col_last_top_skewed[0] << std::endl;
    // std::cout << "out_result_col0_down16 = " << out_result_col0_down16[0] << std::endl;
    // std::cout << "out_result_col0_up16 = " << out_result_col0_up16[0] << std::endl;
    // std::cout << "out_result_col1_down16 = " << out_result_col1_down16[0] << std::endl;
    // std::cout << "out_result_col1_up16 = " << out_result_col1_up16[0] << std::endl;
    price += out_result_col_last_top_skewed[0] + out_result_col0_down16[0] + out_result_col0_up16[0] + out_result_col1_down16[0] + out_result_col1_up16[0];
    price = std::exp(-in_params[0][0] * in_params[0][2]) * (price / (8 * 16 + 5) / in_params[0][4]);
    std::cout << " Price is :" << price << std::endl;
    return 0;
}