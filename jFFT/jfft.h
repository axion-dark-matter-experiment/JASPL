#ifndef JFFT_H
#define JFFT_H

#include <fftw3.h>

#include <stdlib.h>
#include <math.h>

#include <omp.h>
#include <iostream>
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
};

#include "jfft_templates.tpp"

}
#endif // JFFT_H
