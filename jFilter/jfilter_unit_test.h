#ifndef JFILTER_UNIT_TEST_H
#define JFILTER_UNIT_TEST_H

//Header for this file
//
//C System-Headers
#include <math.h>
//C++ System headers
#include <iostream>
#include <chrono>
//OpenCL Headers
//
//Boost Headers
//
//Project specific headers
#include "jVector/jvector.h"
#include "jFFT/ocl_jfft.h"
#include "jFFT/jfft.h"
#include "jFilter/ocl_jlinearfilter.h"
#include "jPlot/jplot.h"
#include "jFilter/jlinearfilter.h"
#include "jAlgorithm/jalgorithm.h"

namespace jaspl {

template <typename T>
class jFilterUnitTest {

  public:
    void CheckFilterCPU( uint num_points ) {

        uint kernel_radius = static_cast<uint>( floor( num_points/100 ) );

        uint kernel_bytes = kernel_radius*2*sizeof( T );

        if( kernel_bytes > CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE ) {
            kernel_radius = static_cast<uint>( CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE/( 2 * sizeof(T) ) );
        }

        JVector<T> signal = SineSignal( num_points );

        std::string mesg = "CPU Linear Convolution Test:";
        mesg += "\nSignal size:";
        mesg += boost::lexical_cast<std::string>( num_points );
        mesg += "\nKernel size:";
        mesg += boost::lexical_cast<std::string>( kernel_radius );

        std::cout << mesg << std::endl;

        TestCPUConvolve( signal, kernel_radius );

    }

    void CheckFilterGPU( uint num_points ) {

        int kernel_radius = static_cast<int>( floor( num_points/100 ) );

        uint kernel_bytes = kernel_radius*2*sizeof( T );

        if( kernel_bytes > CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE ) {
            kernel_radius = static_cast<uint>( CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE/( 2 * sizeof(T) ) );
        }

        JVector<T> signal = SineSignal( num_points );

        std::string mesg = "GPU Linear Convolution Test:";
        mesg += "\nSignal size:";
        mesg += boost::lexical_cast<std::string>( num_points );
        mesg += "\nKernel size:";
        mesg += boost::lexical_cast<std::string>( kernel_radius );

        std::cout << mesg << std::endl;

        TestGPUConvolve( signal, kernel_radius );
    }

  private:

    uint num_elem_per_MB ( uint MB ) {
        return static_cast<uint> (MB/4*1e5);
    }

    inline double gaussian(double x, double sigma) {
        return 1/(sqrt(M_PI_2)*sigma)*exp( -0.5 *pow(x/sigma,2.0));
    }

    JVector<T> HeavisideThetaSignal( uint N ) {

        JVector<T> theta_vect ( N );

        for ( uint i = 0; i < N/2 ; i++ ) {

            theta_vect[i] = 0.0;
        }


        for ( uint i = N/2; i < N ; i++ ) {

            theta_vect[i] = 1.0;
        }

        return theta_vect;
    }

    JVector<T> SineSignal( uint N ) {

        JVector<T> sin_vect ( N );

        for ( uint i = 0; i < N ; i++ ) {

            sin_vect.push_back( sinf( 2*i *2*M_PI/N) + sinf( 25*i*2*M_PI/N) + sinf( 100*i*2*M_PI/N ) );
        }

        return sin_vect;
    }


    void TestCPUConvolve ( jaspl::JVector<T>&vec, uint kernel_radius ) {

        plot( vec, "Original", 500 );

        jaspl::JVector<T> kernel;

        int r = kernel_radius;
        double sigma = static_cast<double>(r)/2.0;

        for( int i = -r; i<= r ; i ++) {
            kernel.push_back(gaussian(i,sigma));
        }

        kernel.Normalize();

        auto start_cpu = std::chrono::high_resolution_clock::now();

        auto convolved_vect = jaspl::JLinearConvolve( vec, kernel );

        plot( convolved_vect, "Convolved (CPU)", 500 );

        auto end_cpu = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> cpu_ms = end_cpu - start_cpu;
        auto time_taken_cpu = cpu_ms.count();

        std::cout<<"CPU took "<<time_taken_cpu<<" ms."<<std::endl;

    }

    void TestGPUConvolve ( jaspl::JVector<T>&vec, int kernel_radius ) {

        plot( vec, "Original", 500 );

        auto convolver = jaspl::ocl::JLinearConvolve();

        jaspl::JVector<T> kernel;

        int r = kernel_radius;
        double sigma = static_cast<double>(r)/2.0;

        for( int i = -r; i<= r ; i ++) {
            kernel.push_back(gaussian(i,sigma));
        }

        kernel.Normalize();

        auto start_gpu = std::chrono::high_resolution_clock::now();

        auto convolved_vect = convolver.Convolve( vec, kernel );

        plot( convolved_vect, "Convolved (GPU)", 500 );

        auto end_gpu = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> gpu_ms = end_gpu - start_gpu;
        auto time_taken_gpu = gpu_ms.count();

        std::cout<<"GPU took "<<time_taken_gpu<<" ms."<<std::endl;

        convolver.TearDown();

    }
};

}

#endif // JFILTER_UNIT_TEST_H
