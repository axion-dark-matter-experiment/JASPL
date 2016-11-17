//Header for this file

//C System-Headers
#include <math.h>
#include <stdio.h>
//C++ System headers
#include <iostream>
#include <chrono>
#include <unistd.h>
//OpenCL Headers

//Boost Headers

//Qt Headers
#include <QApplication>
//Project specific headers
#include "jVector/jvector.h"
#include "jFFT/ocl_jfft.h"
#include "jFFT/jfft.h"
#include "jFFT/jfft_unit_test.h"
#include "jFilter/ocl_jlinearfilter.h"
#include "jPlot/jplot.h"
#include "jFilter/jlinearfilter.h"
#include "jTypeTraits/jtypetraits.h"
#include "jFilter/jfilter_unit_test.h"
#include "jFilter/ocl_jfilter.h"
#include "OpenCLBase/openclbase.h"
#include "TaskQueue/taskqueue.h"
#include "TaskItems/Convolution/convolution.h"
#include "jChart/jchart.h"
#include "TaskItems/FFT/fft.h"
#include "TaskItems/PowerSpectrum/powerspectrum.h"

#define TEST_POINTS 1e5
#define TEST_TYPE float

/*! \mainpage J.A.S.P.L. (Just Another Signal Processing Library)
 *
 * \section intro_sec Introduction
 *
 * JASPL is designed to perform signal processing operations on 1 dimensional signals.
 *
 * \section Dependencies
 *      \li Boost libraries >= 1.62
 *      \li gnuplot-iostream
 *      \li OpenCL installation
 *
 * \section Special Considerations if compling from source
 *      \li OpenMP Installation and compilier that supports OpenMP pragmas
 *
 */

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);

    uint N = TEST_POINTS;
    std::vector< TEST_TYPE > sin_vect;

    for ( uint i = 0; i < N ; i++ ) {

        sin_vect.push_back( sinf( (N/8)*i *2*M_PI/N) + 2*sinf( (N/4)*i*2*M_PI/N) + 3*sinf( (3*N/8)*i*2*M_PI/N ) );
    }

//    JChart test_chart_2;
//    test_chart_2.Plot( sin_vect, "Time Series" );

    jaspl::ocl::TaskQueue< std::vector< TEST_TYPE > > test_q;
    test_q.Load( sin_vect );

    TEST_TYPE fact = static_cast< TEST_TYPE >( 1.0f / 100.0f );

    std::vector< TEST_TYPE > box_vec( 100, fact );
    auto conv_task = jaspl::ocl::Convolution< std::vector< TEST_TYPE > >( box_vec );
    test_q.AddTaskItem( conv_task );

    auto fft = jaspl::ocl::PowerSpectrum< std::vector< TEST_TYPE > >();
    test_q.AddTaskItem( fft );

    test_q.Execute();

    std::vector< TEST_TYPE > processed = test_q.Recall();

    jaspl::plot( sin_vect );
    jaspl::plot( processed );

//    JChart test_chart;
//    test_chart.Plot( processed, "Frequency Domain" );

    return a.exec();

//    jaspl::JFFTUnitTest<float> fft_test;
//    fft_test.TestCPUFFT( TEST_POINTS );
//    fft_test.TestGPUFFT( TEST_POINTS );

//    jaspl::jFilterUnitTest<float> filter_test;
//    filter_test.CheckFilterCPU( TEST_POINTS );
//    filter_test.CheckFilterGPU( TEST_POINTS );
}
