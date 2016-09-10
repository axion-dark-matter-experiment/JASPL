__kernel void Convolve( const __global float* input, __constant float* filter_kernel, __global float* output, const int signal_size, const int kernel_size)
{

    int half_k_size = (kernel_size - 1 )/2;
    int signal_max_index = signal_size - 1;

    int i = get_global_id(0);

    float conv_elem = 0.0;

    int k_max = ( ( i + half_k_size ) > signal_max_index )?( signal_max_index + half_k_size - i ):(kernel_size);
    int k_min = ( ( i - half_k_size ) < 0 )?( half_k_size - i ):(0);

//    printf("%d,%d\n", k_min, k_max);

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

__kernel void LocalConvolve( const __global float* input, __constant float* filter_kernel, __global float* output, const int signal_size, const int kernel_size)
{

    int half_k_size = (kernel_size - 1 )/2;
    int signal_max_index = signal_size - 1;

    int i = get_global_id(0);

    float conv_elem = 0.0f;

    int k_max = ( ( i + half_k_size ) > signal_max_index )?( signal_max_index + half_k_size - i ):(kernel_size);
    int k_min = ( ( i - half_k_size ) < 0 )?( half_k_size - i ):(0);

    __local float local_buffer[ 64 ];

    int group_idx = get_group_id(0);
    int local_idx = get_local_id(0);
    int local_sz = get_local_size(0);

    local_buffer[ local_idx ] = input [ i ];

    barrier(CLK_LOCAL_MEM_FENCE);

    for ( int j = k_min ; j < k_max ; j++ ) {

        if ( (local_idx + j - half_k_size) > 0 ) {
        conv_elem += local_buffer[ local_idx + j - half_k_size ]*filter_kernel[ j ];
        }

    }

//    for ( int j = 0 ; j < k_min ; j++ ) {

//        conv_elem += local_buffer[ -local_idx - j + half_k_size ]*filter_kernel[ j ];
//    }

//    for ( int j = k_max ; j < kernel_size ; j++ ) {

//        conv_elem += local_buffer[ 2*signal_max_index - local_idx - j + half_k_size ]*filter_kernel[ j ];

//    }

    barrier(CLK_LOCAL_MEM_FENCE);

    output[i] = conv_elem;

}

