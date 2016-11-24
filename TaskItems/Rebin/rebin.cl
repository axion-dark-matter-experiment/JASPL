__kernel void PowerSpectrum( __global TYPE* input, __global TYPE* scratch, const unsigned int rebin_size )
{

    int i = get_global_id(0);
    scratch[i] = input[i];

}
