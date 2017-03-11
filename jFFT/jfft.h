#ifndef JFFT_H
#define JFFT_H

//C System-Headers
#include <stdlib.h>
#include <math.h>
//C++ System headers
#include <mutex>
#include <omp.h>
#include <iostream>
#include <atomic>
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
    JFFT( bool use_threading = false );
    ~JFFT();

    T PowerSpectrum( const T& input );
    void SetUp( uint size );

  private:
    void TearDown();

    bool threading = true;
    bool set_up = false;

    std::atomic<uint> N, fft_size;
    typename T::value_type norm_factor;
    fftwf_plan p;
    fftwf_complex *in = NULL;
    fftwf_complex *out = NULL;

    boost::shared_mutex monitor;

};

#include "../../JASPL/jFFT/jfft.tpp"

}
#endif // JFFT_H
