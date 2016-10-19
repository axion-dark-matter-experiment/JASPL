__kernel void TemplatedLinearConvolve( const __global TYPE* input, __constant TYPE* filter_kernel, __global TYPE* output, const int signal_size, const int kernel_size)
{

    int half_k_size = (kernel_size - 1)/2;
    int signal_max_index = signal_size - 1;

    int i = get_global_id(0);

    float conv_elem = 0.0;

    int k_max = ( ( i + half_k_size ) > signal_max_index )?( signal_max_index + half_k_size - i ):(kernel_size);
    int k_min = ( ( i - half_k_size ) < 0 )?( half_k_size - i ):(0);

    for ( int j = k_min ; j < k_max ; j++ ) {

        conv_elem += input[ i + j - half_k_size ]*filter_kernel[ j ];

    }

    for ( int j = 0 ; j < k_min ; j++ ) {

        conv_elem += input[ -i - j + half_k_size ]*filter_kernel[ j ];
    }

    for ( int j = k_max ; j < kernel_size ; j++ ) {

        conv_elem += input[ 2*signal_max_index - i - j + half_k_size ]*filter_kernel[ j ];

    }

    output[i] = conv_elem;
}
