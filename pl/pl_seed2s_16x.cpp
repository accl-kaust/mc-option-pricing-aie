#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

extern "C"
{

    void pl_seed2s_16x(ap_uint<32> mem0,
                     ap_uint<32> mem1,
                     ap_uint<32> mem2,
                     ap_uint<32> mem3,
                     ap_uint<32> mem4,
                     ap_uint<32> mem5,
                     ap_uint<32> mem6,
                     ap_uint<32> mem7,
                     ap_uint<32> mem8,
                     ap_uint<32> mem9,
                     ap_uint<32> mem10,
                     ap_uint<32> mem11,
                     ap_uint<32> mem12,
                     ap_uint<32> mem13,
                     ap_uint<32> mem14,
                     ap_uint<32> mem15,
                     hls::stream<ap_axiu<32, 0, 0, 0>> &s0,
                     hls::stream<ap_axiu<32, 0, 0, 0>> &s1,
                     hls::stream<ap_axiu<32, 0, 0, 0>> &s2,
                     hls::stream<ap_axiu<32, 0, 0, 0>> &s3,
                     hls::stream<ap_axiu<32, 0, 0, 0>> &s4,
                     hls::stream<ap_axiu<32, 0, 0, 0>> &s5,
                     hls::stream<ap_axiu<32, 0, 0, 0>> &s6,
                     hls::stream<ap_axiu<32, 0, 0, 0>> &s7,
                     hls::stream<ap_axiu<32, 0, 0, 0>> &s8,
                     hls::stream<ap_axiu<32, 0, 0, 0>> &s9,
                     hls::stream<ap_axiu<32, 0, 0, 0>> &s10,
                     hls::stream<ap_axiu<32, 0, 0, 0>> &s11,
                     hls::stream<ap_axiu<32, 0, 0, 0>> &s12,
                     hls::stream<ap_axiu<32, 0, 0, 0>> &s13,
                     hls::stream<ap_axiu<32, 0, 0, 0>> &s14,
                     hls::stream<ap_axiu<32, 0, 0, 0>> &s15,
                     int size0,
                     int size1,
                     int size2,
                     int size3,
                     int size4,
                     int size5,
                     int size6,
                     int size7,
                     int size8,
                     int size9,
                     int size10,
                     int size11,
                     int size12,
                     int size13,
                     int size14,
                     int size15)
    {

        ap_uint<32> seed0 = mem0;
        ap_uint<32> seed1 = mem1;
        ap_uint<32> seed2 = mem2;
        ap_uint<32> seed3 = mem3;
        ap_uint<32> seed4 = mem4;
        ap_uint<32> seed5 = mem5;
        ap_uint<32> seed6 = mem6;
        ap_uint<32> seed7 = mem7;
        ap_uint<32> seed8 = mem8;
        ap_uint<32> seed9 = mem9;
        ap_uint<32> seed10 = mem10;
        ap_uint<32> seed11 = mem11;
        ap_uint<32> seed12 = mem12;
        ap_uint<32> seed13 = mem13;
        ap_uint<32> seed14 = mem14;
        ap_uint<32> seed15 = mem15;
    seed2s_loop_0:
        for (int i = 0; i < size0; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = seed0;
            s0.write(x);
            seed0 = seed0 + 1;
        }

    seed2s_loop_1:
        for (int i = 0; i < size1; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = seed1;
            s1.write(x);
            seed1 = seed1 + 2;
        }

    seed2s_loop_2:
        for (int i = 0; i < size2; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = seed2;
            s2.write(x);
            seed2 = seed2 + 3;
        }

    seed2s_loop_3:
        for (int i = 0; i < size3; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = seed3;
            s3.write(x);
            seed3 = seed3 + 4;
        }

    seed2s_loop_4:
        for (int i = 0; i < size4; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = seed4;
            s4.write(x);
            seed4 = seed4 + 5;
        }

    seed2s_loop_5:
        for (int i = 0; i < size5; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = seed5;
            s5.write(x);
            seed5 = seed5 + 6;
        }

    seed2s_loop_6:
        for (int i = 0; i < size6; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = seed6;
            s6.write(x);
            seed6 = seed6 + 7;
        }

    seed2s_loop_7:
        for (int i = 0; i < size7; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = seed7;
            s7.write(x);
            seed7 = seed7 + 8;
        }

    seed2s_loop_8:
        for (int i = 0; i < size8; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = seed8;
            s8.write(x);
            seed8 = seed8 + 9;
        }

    seed2s_loop_9:
        for (int i = 0; i < size9; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = seed9;
            s9.write(x);
            seed9 = seed9 + 10;
        }

    seed2s_loop_10:
        for (int i = 0; i < size10; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = seed10;
            s10.write(x);
            seed10 = seed10 + 11;
        }

    seed2s_loop_11:
        for (int i = 0; i < size11; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = seed11;
            s11.write(x);
            seed11 = seed11 + 12;
        }

    seed2s_loop_12:
        for (int i = 0; i < size12; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = seed12;
            s12.write(x);
            seed12 = seed12 + 13;
        }

    seed2s_loop_13:
        for (int i = 0; i < size13; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = seed13;
            s13.write(x);
            seed13 = seed13 + 14;
        }

    seed2s_loop_14:
        for (int i = 0; i < size14; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = seed14;
            s14.write(x);
            seed14 = seed14 + 15;
        }

    seed2s_loop_15:
        for (int i = 0; i < size15; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = seed15;
            s15.write(x);
            seed15 = seed15 + 16;
        }
    }
}