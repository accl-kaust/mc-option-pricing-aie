#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

extern "C"
{

    void pl_s2mm(ap_int<32> *mem, hls::stream<ap_axiu<32, 0, 0, 0>> &s, int size)
    {
    s2mm_loop:
        for (int i = 0; i < size; i++)
        {
            ap_axiu<32, 0, 0, 0> x = s.read();
            mem[i] = x.data;
        }
    }
}