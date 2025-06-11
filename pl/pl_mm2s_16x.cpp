#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

extern "C"
{

    void pl_mm2s_16x(ap_int<32> *mem0,
                     ap_int<32> *mem1,
                     ap_int<32> *mem2,
                     ap_int<32> *mem3,
                     ap_int<32> *mem4,
                     ap_int<32> *mem5,
                     ap_int<32> *mem6,
                     ap_int<32> *mem7,
                     ap_int<32> *mem8,
                     ap_int<32> *mem9,
                     ap_int<32> *mem10,
                     ap_int<32> *mem11,
                     ap_int<32> *mem12,
                     ap_int<32> *mem13,
                     ap_int<32> *mem14,
                     ap_int<32> *mem15,
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
    mm2s_loop_0:
        for (int i = 0; i < size0; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = mem0[i];
            s0.write(x);
        }

    mm2s_loop_1:
        for (int i = 0; i < size1; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = mem1[i];
            s1.write(x);
        }

    mm2s_loop_2:
        for (int i = 0; i < size2; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = mem2[i];
            s2.write(x);
        }

    mm2s_loop_3:
        for (int i = 0; i < size3; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = mem3[i];
            s3.write(x);
        }

    mm2s_loop_4:
        for (int i = 0; i < size4; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = mem4[i];
            s4.write(x);
        }

    mm2s_loop_5:
        for (int i = 0; i < size5; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = mem5[i];
            s5.write(x);
        }

    mm2s_loop_6:
        for (int i = 0; i < size6; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = mem6[i];
            s6.write(x);
        }

    mm2s_loop_7:
        for (int i = 0; i < size7; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = mem7[i];
            s7.write(x);
        }

    mm2s_loop_8:
        for (int i = 0; i < size8; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = mem8[i];
            s8.write(x);
        }

    mm2s_loop_9:
        for (int i = 0; i < size9; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = mem9[i];
            s9.write(x);
        }

    mm2s_loop_10:
        for (int i = 0; i < size10; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = mem10[i];
            s10.write(x);
        }

    mm2s_loop_11:
        for (int i = 0; i < size11; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = mem11[i];
            s11.write(x);
        }

    mm2s_loop_12:
        for (int i = 0; i < size12; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = mem12[i];
            s12.write(x);
        }

    mm2s_loop_13:
        for (int i = 0; i < size13; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = mem13[i];
            s13.write(x);
        }

    mm2s_loop_14:
        for (int i = 0; i < size14; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = mem14[i];
            s14.write(x);
        }

    mm2s_loop_15:
        for (int i = 0; i < size15; i++)
        {
            ap_axiu<32, 0, 0, 0> x;
            x.data = mem15[i];
            s15.write(x);
        }
    }
}