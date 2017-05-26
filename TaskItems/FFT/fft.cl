__kernel void PowerSpectrum( __global TYPE* input, __global TYPE* scratch, uint signal_size )
{

    int i = get_global_id(0);
    //Do not use sqrt(), you -will- get round-off errors for large FFT sizes
    scratch[i] = input[2*i]*input[2*i] + input[2*i+1]*input[2*i+1];
    //Normalize by number of elements
    scratch[i] /= (TYPE)signal_size * (TYPE)signal_size;


    TYPE N_sqr = (TYPE)signal_size * (TYPE)signal_size;

    // Explicitly compute first element
    TYPE C_0 = input[0]*input[0];
    C_0 /= N_sqr;

    scratch[0] = C_0;

    TYPE C_i = abs_sqr( input[i] ) + abs_sqr( input[signal_size-i] );;
    C_i /= N_sqr;

    scratch[i] = C_i;

    // Explicitly compute last element
    TYPE C_N_half = abs_sqr( input[ signal_size - 1 ] );
    C_N_half /= N_sqr;

    scratch[ signal_size - 1 ] = C_N_half;

}
