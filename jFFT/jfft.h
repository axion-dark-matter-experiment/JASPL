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

class JFFT {
  public:
    JFFT( bool use_threads = true );

    void SetUp(uint size);
    template <typename T> void PowerSpectrum( T &input );
    void TearDown();

  private:
    bool threading = true;

    uint N;
    float N_f;
    fftw_plan p;
    fftw_complex *in = NULL;
    fftw_complex *out = NULL;

    boost::shared_mutex monitor;

};

#include "jfft_templates.tpp"

}
#endif // JFFT_H
