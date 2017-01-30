#ifndef JFILTER_H
#define JFILTER_H

//Header for this file
//
//C System-Headers
//
//C++ System headers
#include <mutex>
//OpenCL Headers
//
//Boost Headers
//
//OpenMP Headers
#include<omp.h>
//Project specific headers
#include "../jTypeTraits/jtypetraits.h"
#include "../JASPL/jVector/jvector.h"

namespace jaspl {

class JFilter {
  public:
    JFilter();

};

template <typename T>
T JLinearConvolve( T& signal, T& kernel) {

    static_assert( std::is_pointer<T>::value || is_stdlib_container<T>::value, "JLinearConvolve:: signal and kernel must be pointer of container-like objects.");

    int kernel_size = kernel.size();
    int half_k_size = (kernel_size - 1 )/2;

    int signal_size = signal.size();
    int signal_max_index = signal_size - 1;

    T output( signal.size(), 0.0f );

    #pragma omp parallel for
    for ( int i = 0 ; i < signal_size ; i++ ) {

        float conv_elem = 0.0f;

        int k_max = ( ( i + half_k_size ) > signal_max_index )?( signal_max_index + half_k_size - i ):(kernel_size);
        int k_min = ( ( i - half_k_size ) < 0 )?( half_k_size - i ):(0);

        for ( int j = k_min ; j < k_max ; j++ ) {

            conv_elem += signal[ i + j - half_k_size]*kernel[ j ];
        }

        for ( int j = 0 ; j < k_min ; j++ ) {

            conv_elem += signal[ -i - j + half_k_size ]*kernel[ j ];
        }

        for ( int j = k_max ; j < kernel_size ; j++ ) {

            conv_elem += signal[ 2*signal_max_index - i - j + half_k_size ]*kernel[ j ];

        }

        output[i] = conv_elem;

    }

    return output;
}

}

#endif // JFILTER_H
