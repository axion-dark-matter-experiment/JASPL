//Header for this file

//C System-Headers
#include <math.h>
//C++ System headers
#include <iostream>
#include <chrono>
//OpenCL Headers

//Boost Headers

//Project specific headers
#include "jVector/jvector.h"
#include "jFFT/ocl_jfft.h"
#include "jFFT/jfft.h"
#include "jFFT/jfft_unit_test.h"
#include "jFilter/ocl_jfilter.h"
#include "jPlot/jplot.h"
#include "jFilter/jfilter.h"
#include "jAlgorithm/jalgorithm.h"
#include "jFilter/jfilter_unit_test.h"
#include "OpenCLBase/openclbase.h"

#define TEST_POINTS 1e7

/*! \mainpage J.A.S.P.L. (Just Another Signal Processing Library)
 *
 * \section intro_sec Introduction
 *
 * JASPL is designed to perform basic operations on one dimensional signals.
 *
 * \section Dependencies
 *      \li Boost libraries >= 1.62
 *      \li gnuplot-iostream
 *      \li OpenCL installation
 *
 */

int main() {

    jaspl::JFFTUnitTest<float> fft_test;
    fft_test.TestCPUFFT( TEST_POINTS );
    fft_test.TestGPUFFT( TEST_POINTS );

    jaspl::jFilterUnitTest<float> filter_test;
    filter_test.CheckFilterCPU( TEST_POINTS );
    filter_test.CheckFilterGPU( TEST_POINTS );

    return 0;
}
