//Header for this file
#include "jfft.h"
//C System-Headers
//
//C++ System headers
//
//OpenCL Headers
//
//Boost Headers
//
//Project specific headers
//#include <jAlgorithm/jalgorithm.h>

namespace jaspl {

JFFT::JFFT(bool use_threads) : threading( use_threads  ) {

}

void JFFT::SetUp( uint size ) {

    N = size;
    N_f = static_cast<float>( N );
    //Setup multi-threading
    if ( threading ) {

        int val = fftw_init_threads();

        //if val is not equal to zero multithreading cannot be setup
        if( val == 0 ) {
            std::string err_mesg = __func__;
            err_mesg += "Could not setup multithreading.";
            throw std::runtime_error(err_mesg);
        }
        //set plans to use what OpenMP has decided is a good number of
        //threads (usually the number of processor cores)
        fftw_plan_with_nthreads(omp_get_max_threads());

    }

    if( in != NULL ) {
        fftw_free( in );
    }

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    if( out != NULL ) {
        fftw_free( out );
    }

    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

}

void JFFT::TearDown() {

    fftw_destroy_plan(p);

    fftw_free(in);
    fftw_free(out);
}

}
