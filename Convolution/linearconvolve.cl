#define SIGNAL_SIZE 1604
#define KERNEL_SIZE 100

__kernel void LinearConvolve( __global TYPE* input, __constant TYPE* filter_kernel )
{

    int half_k_size = (KERNEL_SIZE - 1)/2;
    int signal_max_index = SIGNAL_SIZE - 1;

    int i = get_global_id(0);

    TYPE conv_elem = 0.0;

    int k_max = ( ( i + half_k_size ) > signal_max_index )?( signal_max_index + half_k_size - i ):(KERNEL_SIZE);
    int k_min = ( ( i - half_k_size ) < 0 )?( half_k_size - i ):(0);

    for ( int j = k_min ; j < k_max ; j++ ) {

        conv_elem += input[ i + j - half_k_size ]*filter_kernel[ j ];

    }

    for ( int j = 0 ; j < k_min ; j++ ) {

        conv_elem += input[ -i - j + half_k_size ]*filter_kernel[ j ];
    }

    for ( int j = k_max ; j < KERNEL_SIZE ; j++ ) {

        conv_elem += input[ 2*signal_max_index - i - j + half_k_size ]*filter_kernel[ j ];

    }

    input[i] = conv_elem;

}

__kernel void Swap( __global TYPE* scratch_space, __global TYPE* processed ) {

    int i = get_global_id(0);
    scratch_space[i] = processed[i];

}

//__kernel void LinearConvolve( __global float* TYPE, __constant TYPE* filter_kernel )
//{

//    int i = get_global_id(0);
////    printf( "%d", i );
//    input[i] = 10.0f;

//}
