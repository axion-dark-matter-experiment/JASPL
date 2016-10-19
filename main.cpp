//Header for this file

//C System-Headers
#include <math.h>
//C++ System headers
#include <iostream>
#include <chrono>
//OpenCL Headers

//Boost Headers

//Project specific headers
#include "jvector.h"
#include "ocl_jfft.h"
#include "jfft.h"
#include "ocl_jfilter.h"
#include "jplot.h"
#include "jfilter.h"

#define TEST_TYPE float

template <typename T> void TestCPUFFT( jaspl::JVector<T>& vec ) {

    auto fft_er = jaspl::JFFT();

    fft_er.Setup( vec.size() );

    auto start_cpu = std::chrono::high_resolution_clock::now();

    fft_er.PowerSpectrum( vec );

    auto end_cpu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> cpu_ms = end_cpu - start_cpu;
    auto time_taken_cpu = cpu_ms.count();

    std::cout<<"CPU took "<<time_taken_cpu<<" ms."<<std::endl;

    fft_er.TearDown();

}

template <typename T> void TestGPUFFT( jaspl::JVector<T>&vec ) {

    auto ocl_fft_er = jaspl::ocl::JFFT();

    ocl_fft_er.Setup();

    auto start_gpu = std::chrono::high_resolution_clock::now();

    ocl_fft_er.PowerSpectrum( vec );

    auto end_gpu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> gpu_ms = end_gpu - start_gpu;
    auto time_taken_gpu = gpu_ms.count();

    ocl_fft_er.Teardown();

    std::cout<<"GPU took "<<time_taken_gpu<<" ms."<<std::endl;

}

uint num_elem_per_MB ( uint MB ) {
    return static_cast<uint> (MB/4*1e5);
}

inline double gaussian(double x, double sigma) {
    return 1/(sqrt(M_PI_2)*sigma)*exp( -0.5 *pow(x/sigma,2.0));
}

template <typename T> void TestCPUConvolve ( jaspl::JVector<T>&vec, int kernel_radius ) {

    plot( vec, "Original", 500 );

    jaspl::JVector<TEST_TYPE> kernel;

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

template <typename T> void TestGPUConvolve ( jaspl::JVector<T>&vec, int kernel_radius ) {

    plot( vec, "Original", 500 );

    auto convolver = jaspl::ocl::JLinearConvolve();

    jaspl::JVector<TEST_TYPE> kernel;

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

int main() {

    uint N = static_cast<uint>( 1e6 );

    jaspl::JVector<TEST_TYPE> sin_vect ( N );

    #pragma omp parallel for ordered
    for ( uint i = 0; i < N ; i++ ) {

        #pragma omp ordered
        sin_vect.push_back( sinf( 2*i *2*M_PI/N) + sinf( 25*i*2*M_PI/N) + sinf( 100*i*2*M_PI/N ) );
    }

//    for ( uint i = 0; i < N/2 ; i++ ) {

//        sin_vect.push_back( 0.0 );
//    }


//    for ( uint i = N/2; i < N ; i++ ) {

//        sin_vect.push_back( 1.0 );
//    }

    TestGPUConvolve(sin_vect, 5*1e3);
    TestCPUConvolve(sin_vect, 5*1e3);
//    TestGPUFFT( sin_vect );
//    TestCPUFFT( sin_vect );

    return 0;
}
