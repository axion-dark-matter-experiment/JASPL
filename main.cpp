
//C System-Headers
#include <math.h>
#include <stdio.h>
//C++ System headers
#include <iostream>
#include <chrono>
#include <unistd.h>
//OpenCL Headers
//
//Boost Headers
//
//Qt Headers
//
//Project specific headers
#include "jFFT/test_jfft.h"
#include "jVector/jvector.h"
#include "jFFT/jfft.h"
#include "jFFT/perf_jfft.h"
#include "jPlot/jplot.h"
#include "jFilter/jlinearfilter.h"
#include "jTypeTraits/jtypetraits.h"
#include "jFilter/jfilter_unit_test.h"
#include "OpenCLBase/openclbase.h"
#include "TaskQueue/taskqueue.h"
#include "TaskItems/LinearConvolution/linearconvolution.h"
#include "jChart/jchart.h"
#include "TaskItems/FFT/fft.h"
#include "TaskItems/PowerSpectrum/powerspectrum.h"
#include "TaskItems/Arithmetic/ScalarAdd/scalaradd.h"
#include "TaskItems/Arithmetic/ScalarMultiply/scalarmultiply.h"
#include "Containers/test_ouroborus.h"

#define TEST_POINTS pow( 2, 16 )
#define TEST_TYPE float
#define MAX_ITER 24

void TimingTest() {
    //  QApplication a(argc, argv);

    uint N = (uint)TEST_POINTS;

    std::vector< TEST_TYPE > sin_vect;
    sin_vect.reserve( N );

    for ( uint i = 0; i < N ; i++ ) {

        sin_vect.push_back( sinf( (N/8)*i *2*M_PI/N) + 2*sinf( (N/4)*i*2*M_PI/N) + 3*sinf( (3*N/8)*i*2*M_PI/N ) );
        //        uint M = 500;
        //        sin_vect.push_back( sinf( (M)*i *2*M_PI/N) + 2*sinf( (2*M)*i*2*M_PI/N) + 3*sinf( (3*M)*i*2*M_PI/N ) );
    }

    //    jaspl::plot( sin_vect );

    //    auto start_opencl = std::chrono::high_resolution_clock::now();

    auto test_q = jaspl::ocl::TaskQueue< std::vector< TEST_TYPE > > ( 0, 0 );

    TEST_TYPE fact = static_cast< TEST_TYPE >( 1.0f / 100.0f );

    std::vector< TEST_TYPE > box_vec( 100, fact );
    jaspl::ocl::LinearConvolution< std::vector< TEST_TYPE > > conv_task ( box_vec );
    test_q.AddTaskItem( conv_task );

    auto fft_task = jaspl::ocl::PowerSpectrum< std::vector< TEST_TYPE > > ();
    test_q.AddTaskItem( fft_task );

    auto mult_task = jaspl::ocl::ScalarMultiply< std::vector< TEST_TYPE > >( 2.0f );
    test_q.AddTaskItem( mult_task );

    auto addition_task = jaspl::ocl::ScalarAdd< std::vector< TEST_TYPE > >( 5.0f );
    test_q.AddTaskItem( addition_task );

    auto start_opencl = std::chrono::high_resolution_clock::now();

    test_q.Load( sin_vect );

    test_q.Execute();

    //    test_q.PrintContents();

    std::vector< TEST_TYPE > processed = test_q.Recall();

    auto end_opencl = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> opencl_ms = end_opencl - start_opencl;
    auto time_taken_opencl = opencl_ms.count();

    std::cout<<"OpenCL Took "<<time_taken_opencl<<" ms."<<std::endl;

    jaspl::plot( processed, "OpenCL" );

    //  return a.exec();

    jaspl::JFFT< std::vector < TEST_TYPE > > fft_er( true );

    auto time_series = sin_vect;

    auto start_cpu = std::chrono::high_resolution_clock::now();

    fft_er.PowerSpectrum( time_series );

    uint spectrum_size = time_series.size();
    uint n_half = (spectrum_size % 2 == 0) ? (spectrum_size / 2) : ((
                      spectrum_size - 1) / 2);

    time_series.erase(time_series.end() - n_half , time_series.end());

    auto end_cpu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> cpu_ms = start_cpu - end_cpu;
    auto time_taken_cpu = cpu_ms.count();

    std::cout << "CPU Took" << time_taken_cpu << " ms." << std::endl;

    //    jaspl::plot( time_series, "CPU" );
}

void fftTimingTest() {

    std::vector< double > opencl_timing_data;
    std::vector< double > cpu_timing_data;

    auto test_q = jaspl::ocl::TaskQueue< std::vector< TEST_TYPE > > ( 0, 0 );

    auto fft = jaspl::ocl::FFT< std::vector< TEST_TYPE > >();
    test_q.AddTaskItem( fft );

    for ( uint j = 2 ; j <= MAX_ITER ; j ++ ) {

        uint N = static_cast<uint>( pow( 2.0, j ) );

        std::cout << "Timing signal size of " << N << std::endl;

        std::vector< TEST_TYPE > sin_vect;
        sin_vect.reserve( N );

        for ( uint i = 0; i < N ; i++ ) {
            sin_vect.push_back( sinf( (N/8)*i *2*M_PI/N) + 2*sinf( (N/4)*i*2*M_PI/N) + 3*sinf( (3*N/8)*i*2*M_PI/N ) );
        }

        auto start_opencl = std::chrono::high_resolution_clock::now();

        test_q.Load( sin_vect );

        test_q.Execute();

        std::vector< TEST_TYPE > processed = test_q.Recall();

        auto end_opencl = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> opencl_ms = end_opencl - start_opencl;
        auto time_taken_opencl = opencl_ms.count();

        std::cout<<"OpenCL Took "<<time_taken_opencl<<" ms."<<std::endl;

        opencl_timing_data.push_back( time_taken_opencl );

    }

    for ( uint j = 2 ; j <= MAX_ITER ; j ++ ) {

        uint N = static_cast<uint>( pow( 2.0, j ) );

        std::cout << "Timing signal size of " << N << std::endl;

        std::vector< TEST_TYPE > sin_vect;
        sin_vect.reserve( N );

        for ( uint i = 0; i < N ; i++ ) {
            sin_vect.push_back( sinf( (N/8)*i *2*M_PI/N) + 2*sinf( (N/4)*i*2*M_PI/N) + 3*sinf( (3*N/8)*i*2*M_PI/N ) );
        }

        auto start_cpu = std::chrono::high_resolution_clock::now();

        auto fft_er = std::unique_ptr< jaspl::JFFT< std::vector< TEST_TYPE > > >( new jaspl::JFFT< std::vector< TEST_TYPE > >( true ) );

        auto power_spec = fft_er->PowerSpectrum( sin_vect );

        uint spectrum_size = sin_vect.size();
        uint n_half = (spectrum_size % 2 == 0) ? (spectrum_size / 2) : (( spectrum_size - 1) / 2);

        power_spec.erase( power_spec.end() - n_half , power_spec.end());

        auto end_cpu = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> cpu_ms = end_cpu - start_cpu;
        auto time_taken_cpu = cpu_ms.count();

        std::cout << "CPU Took" << time_taken_cpu << " ms." << std::endl;

        cpu_timing_data.push_back( time_taken_cpu );
    }

//    jaspl::plot_to_disk( cpu_timing_data,
//                         opencl_timing_data,
//                         "Intel(R) Core(TM) i7-4700HQ CPU @ 2.40GHz","GeForce GTX 860M",
//                         "Power Spectrum CPU vs. GPU" );

}

void convolutionTimingTest() {

    std::vector< double > opencl_timing_data;
    std::vector< double > cpu_timing_data;

    auto test_q = jaspl::ocl::TaskQueue< std::vector< TEST_TYPE > > ( 0, 0 );

    TEST_TYPE fact = static_cast< TEST_TYPE >( 1.0f / 100.0f );

    std::vector< TEST_TYPE > box_vec( 100, fact );

    auto conv_task = jaspl::ocl::LinearConvolution< std::vector< TEST_TYPE > >( box_vec );
    test_q.AddTaskItem( conv_task );

    for ( uint j = 2 ; j <= MAX_ITER ; j ++ ) {

        uint N = static_cast<uint>( pow( 2.0, j ) );

        std::cout << "Timing signal size of " << N << std::endl;

        std::vector< TEST_TYPE > sin_vect;
        sin_vect.reserve( N );

        for ( uint i = 0; i < N ; i++ ) {
            sin_vect.push_back( sinf( (N/8)*i *2*M_PI/N) + 2*sinf( (N/4)*i*2*M_PI/N) + 3*sinf( (3*N/8)*i*2*M_PI/N ) );
        }

        auto start_opencl = std::chrono::high_resolution_clock::now();

        test_q.Load( sin_vect );

        test_q.Execute();

        std::vector< TEST_TYPE > processed = test_q.Recall();

        auto end_opencl = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> opencl_ms = end_opencl - start_opencl;
        auto time_taken_opencl = opencl_ms.count();

        std::cout<<"OpenCL Took "<<time_taken_opencl<<" ms."<<std::endl;

        opencl_timing_data.push_back( time_taken_opencl );

    }

    for ( uint j = 2 ; j <= MAX_ITER ; j ++ ) {

        uint N = static_cast<uint>( pow( 2.0, j ) );

        std::cout << "Timing signal size of " << N << std::endl;

        std::vector< TEST_TYPE > sin_vect;
        sin_vect.reserve( N );

        for ( uint i = 0; i < N ; i++ ) {
            sin_vect.push_back( sinf( (N/8)*i *2*M_PI/N) + 2*sinf( (N/4)*i*2*M_PI/N) + 3*sinf( (3*N/8)*i*2*M_PI/N ) );
        }

        auto start_cpu = std::chrono::high_resolution_clock::now();

        jaspl::JLinearConvolve( sin_vect, box_vec );

        auto end_cpu = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> cpu_ms = end_cpu - start_cpu;
        auto time_taken_cpu = cpu_ms.count();

        std::cout << "CPU Took" << time_taken_cpu << " ms." << std::endl;

        cpu_timing_data.push_back( time_taken_cpu );
    }

//    jaspl::plot_to_disk( cpu_timing_data,
//                         opencl_timing_data,
//                         "Intel(R) Core(TM) i7-4700HQ CPU @ 2.40GHz","GeForce GTX 860M",
//                         "Linear Convolution CPU vs. GPU" );

}

int main(int argc, char *argv[]) {

    (void)argc;
    (void)argv;

//    jaspl::ocl::PrintOCLDebugInfo();

//    fftTimingTest();
//    convolutionTimingTest();
    jaspl::RunTestJFFT();

}
