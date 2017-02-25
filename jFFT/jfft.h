#ifndef JFFT_H
#define JFFT_H

//C System-Headers
#include <stdlib.h>
#include <math.h>
//C++ System headers
#include <mutex>
#include <omp.h>
#include <iostream>
// FFTW Headers
#include <fftw3.h>
// Boost Headers
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>
//Project specific headers
#include "../JASPL/jTypeTraits/jtypetraits.h"

namespace jaspl {

template < typename T >
class JFFT {
  public:
    JFFT( uint transform_size, bool use_threading = false );
    ~JFFT();

    T PowerSpectrum( const T& input );

  private:
    void SetUp( uint size );
    void TearDown();

    bool threading = true;

    uint N, fft_size;
    typename T::value_type norm_factor;
    fftw_plan p;
    fftw_complex *in = NULL;
    fftw_complex *out = NULL;

    boost::shared_mutex monitor;

};

#include "jfft.tpp"

}
#endif // JFFT_H
