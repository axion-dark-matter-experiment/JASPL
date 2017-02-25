#ifndef JFFT_UNIT_TEST_H
#define JFFT_UNIT_TEST_H

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
#include "jFFT/jfft.h"
#include "jPlot/jplot.h"

namespace jaspl {

std::vector< float > build_test_signal( uint N ) {

    std::vector< float > sin_vect ( N );

    for ( uint i = 0; i < N ; i++ ) {

        sin_vect.push_back( sinf( (5)*i *2*M_PI/N) + 2*sinf( (25)*i*2*M_PI/N) + 3*sinf( (50)*i*2*M_PI/N ) );
    }

    return sin_vect;
}

void PerfJFFT() {

    const uint signal_size = 10000;

    JFFT< std::vector < float > > fft_er( signal_size, true );

    std::vector< float > signal = build_test_signal( signal_size );

    auto start_cpu = std::chrono::high_resolution_clock::now();

    fft_er.PowerSpectrum( signal );

    auto end_cpu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> cpu_ms = end_cpu - start_cpu;
    auto time_taken_cpu = cpu_ms.count();

    std::cout<<"CPU took "<<time_taken_cpu<<" ms."<<std::endl;

}

}

#endif // JFFT_UNIT_TEST_H
