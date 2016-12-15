__kernel void PowerSpectrum( __global TYPE* input, __global TYPE* scratch, uint signal_size )
{

    int i = get_global_id(0);
    //Do not use sqrt(), you -will- get round-off errors for large FFT sizes
    scratch[i] = input[2*i]*input[2*i] + input[2*i+1]*input[2*i+1];
    //Normalize by number of elements
    scratch[i] /= (TYPE)signal_size * (TYPE)signal_size;

}
