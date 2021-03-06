#ifndef TEST_JFFT_H
#define TEST_JFFT_H

//C System-Headers
//
//C++ System headers
#include <cmath>
#include <limits>
#include <iomanip>
#include <iostream>
#include <type_traits>
#include <algorithm>
#include <vector>
// FFTW Headers
//
// Boost Headers
//
// Google Test
#include <gtest/gtest.h>
//Project specific headers
#include "jfft.h"
#include "../jPlot/jplot.h"

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
almost_equal(T x, T y, int ulp) {
    // the machine epsilon has to be scaled to the magnitude of the values used
    // and multiplied by the desired precision in ULPs (units in the last place)
    return std::abs(x-y) < std::numeric_limits<T>::epsilon() * std::abs(x+y) * ulp
           // unless the result is subnormal
           || std::abs(x-y) < std::numeric_limits<T>::min();
}

double sqr_sum( const std::vector< double >& signal ) {

    double tot = 0.0;

    for ( const auto& val : signal ) {
        tot += val*val;
    }

    return tot;
}

double sum( const std::vector< double >& signal ) {

    double tot = 0.0;

    for ( const auto& val : signal ) {
        tot += val;
    }

    return tot;
}

std::vector< double > build_test_signal( uint N ) {

    std::vector< double > sin_vect;
    sin_vect.reserve( N );

    double N_f = static_cast<double>( N );

    for ( uint i = 0; i < N ; i++ ) {

        double x = static_cast< double >( i )/N_f;

        sin_vect.push_back( 0.25*std::sin( (5.0)*(2.0*M_PI)*x ) );
    }

    return sin_vect;
}

namespace jaspl {

void TestJFFT( const uint signal_size ) {

    JFFT< std::vector< double > > fft_er( true );

    std::vector< double > signal = build_test_signal( signal_size );

    double time_series_tot_pow = sqr_sum( signal );

    plot( signal, "Times Series" );

    fft_er.SetUp( signal_size );

    std::vector< double > power_spectrum = fft_er.PowerSpectrum( signal );

    double power_spectrum_tot_pow = 2.0*sum( power_spectrum );

    std::for_each(power_spectrum.begin(), power_spectrum.end(), [=](double &x){std::sqrt(x); });

    plot( power_spectrum, "Power Spectrum" );


    if( !almost_equal( time_series_tot_pow, power_spectrum_tot_pow, 2 ) ) {

        std::cout << "Parseval's theorem has been violated." << std::endl;

        std::cout << "Square sum of time series values: "
                  << time_series_tot_pow
                  << " Square sum of power spectrum values: "
                  << power_spectrum_tot_pow
                  << "\n"
                  << std::endl;

        std::cout << "Power spectrum differs by multiplicative factor of: "
                  << power_spectrum_tot_pow/time_series_tot_pow
                  << std::endl;

    } else {

        std::cout << "Parseval's theorem holds." << std::endl;

        std::cout << "Square sum of time series values: "
                  << time_series_tot_pow
                  << " Square sum of power spectrum values: "
                  << power_spectrum_tot_pow
                  << "\n"
                  << std::endl;
    }

}

void RunTestJFFT() {

    for ( uint i = 2 ; i < std::pow( 2, 12 ); i *= 2 ) {

        std::cout << "Testing JFFT for signal of size "
                  << i
                  << std::endl;

        TestJFFT( i );
    }
}

}

#endif // TEST_JFFT_H
