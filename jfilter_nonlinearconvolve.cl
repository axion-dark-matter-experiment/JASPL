//define a gaussian function with standard deviation sigma and a mean value of zero
//used in the construction of gaussian kernels
inline float gaussian(double x, double sigma) {
    return 1/(sqrt(M_PI_2)*sigma)*exp( -0.5 *pow(x/sigma,2.0));
}

__kernel void NonLinearConvolve( const __global float* input, __constant float* filter_kernel, __global float* output, const int signal_size, const int kernel_size)
{

    int half_k_size = (kernel_size - 1)/2;
    int signal_max_index = signal_size - 1;

    int i = get_global_id(0);

    float conv_elem = 0.0;
    float norm_factor = 0.0;

    int k_max = ( ( i + half_k_size ) > signal_max_index )?( signal_max_index + half_k_size - i ):(kernel_size);
    int k_min = ( ( i - half_k_size ) < 0 )?( half_k_size - i ):(0);

    for ( int j = k_min ; j < k_max ; j++ ) {

        float spatial_weight = abs( i - j );
        float range_weight = input[ i ] - input [ j ];
        float spatial_function_val = SPATIAL_FUNCTION( spatial_weight );
        float range_function_val = RANGE_FUNCTION( range_weight );

        norm_factor += spatial_function_val*range_function_val;
        conv_elem += input[ i + j - half_k_size ]*spatial_function_val*range_function_val;

    }

//    for ( int j = 0 ; j < k_min ; j++ ) {

//        conv_elem += input[ -i - j + half_k_size ]*filter_kernel[ j ];
//    }

//    for ( int j = k_max ; j < kernel_size ; j++ ) {

//        conv_elem += input[ 2*signal_max_index - i - j + half_k_size ]*filter_kernel[ j ];
//    }

    output[i] = (conv_elem/norm_factor);
}


for(int p = radius; p < list_size + radius; p++) {

    double conv_element = 0.0;
    double norm_weight = 0.0;

    for(int q = p -radius; q <= p + radius ; q++) {

        double pos_weight = abs(p-q);
        double intens_weight = data_deque.at(p)-data_deque.at(q);
        double val_sigma_s = gaussian(pos_weight,sigma_s);
        double val_sigma_r = gaussian(intens_weight,sigma_r);
        norm_weight +=val_sigma_s*val_sigma_r;
        conv_element +=val_sigma_s*val_sigma_r*data_deque.at(q);

    }

    #pragma omp ordered
    convolved_list.push_back(conv_element/norm_weight);
}
