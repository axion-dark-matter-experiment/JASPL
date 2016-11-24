INCLUDEPATH += ../../../include
LIBS += -L$$OUT_PWD/../../../lib

TEMPLATE = app

QT += charts
QT += core gui widgets

CONFIG += c++14

LIBS += -L/usr/local/lib -L/usr/lib -lboost_iostreams -lboost_system -lboost_filesystem

INCLUDEPATH +=-I "/usr/local/cuda/include"
LIBS +=-L "/usr/local/cuda/lib64" -lOpenCL

INCLUDEPATH +=-I/usr/local/include
LIBS +=-L/usr/local/lib64 -lclFFT

LIBS +=-lfftw3_threads -lfftw3 -lm

QMAKE_CXXFLAGS+= -fopenmp
#QMAKE_CXXFLAGS+= -O3
QMAKE_LFLAGS +=  -fopenmp

SOURCES += main.cpp \
    jTypeTraits/jalgorithm.cpp \
    jFFT/jfft.cpp \
    jFFT/jfft_templates.tpp \
    jFFT/ocl_jfft.cpp \
    jFFT/ocl_jfft_templates.tpp \
    jPlot/jplot.cpp \
    jVector/jvector.cpp \
    jVector/jvector_templates.tpp \
    OpenCLBase/openclbase.cpp \
    jFFT/jfft_unit_test.cpp \
    jFilter/jnonlinearfilter.cpp \
    jFilter/ocl_jnonlinearfilter.cpp \
    jFilter/ocl_jlinearfilter_templates.tpp \
    jFilter/jlinearfilter.cpp \
    jFilter/ocl_jlinearfilter.cpp \
    jFilter/ocl_jfilter.cpp \
    jFilter/ocl_jfilter_templates.tpp \
    TaskQueue/taskqueue.cpp \
    TaskQueue/taskqueue_templates.tpp \
    TaskItem/taskitem.cpp \
    TaskItem/taskitem_templates.tpp \
    jChart/jchart.cpp \
    jChart/jchart_templates.tpp \
    TaskQueue/taskqueuebase.cpp \
    TaskItems/LinearConvolution/linearconvolution.cpp \
    TaskItems/LinearConvolution/linearconvolution.tpp \
    TaskItems/FFT/fft.cpp \
    TaskItems/FFT/fft.tpp \
    TaskItems/PowerSpectrum/powerspectrum.cpp \
    TaskItems/PowerSpectrum/powerspectrum.tpp \
#    TaskItems/NonLinearConvolution/nonlinearconvolution.cpp \
#    TaskItems/NonLinearConvolution/nonlinearconvolution.tpp \
    TaskItems/Rebin/rebin.cpp

HEADERS += \
    jFFT/jfft.h \
    jFFT/ocl_jfft.h \
    jPlot/jplot.h \
    jVector/jvector.h \
    jFilter/jfilter_unit_test.h \
    OpenCLBase/openclbase.h \
    jFFT/jfft_unit_test.h \
    jFilter/jnonlinearfilter.h \
    jFilter/ocl_jnonlinearfilter.h \
    jFilter/ocl_jlinearfilter.h \
    jFilter/jlinearfilter.h \
    jFilter/ocl_jfilter.h \
    TaskQueue/taskqueue.h \
    TaskItem/taskitem.h \
    jChart/jchart.h \
    TaskQueue/taskqueuebase.h \
    TaskItems/LinearConvolution/linearconvolution.h \
    TaskItems/FFT/fft.h \
    TaskItems/PowerSpectrum/powerspectrum.h \
    jTypeTraits/jtypetraits.h \
#    TaskItems/NonLinearConvolution/nonlinearconvolution.h \
    TaskItems/Rebin/rebin.h

DISTFILES += \
    jFilter/jfilter_linearconvolve.cl \
    jFilter/jfilter_nonlinearconvolve.cl \
    jFilter/jfilter_templated_linearconvolve.cl \
    TaskItems/LinearConvolution/linearconvolve.cl \
    TaskItems/FFT/fft.cl \
    TaskItems/PowerSpectrum/powerspectrum.cl \
#    TaskItems/NonLinearConvolution/nonlinearconvolve.cl \
    TaskItems/Rebin/rebin.cl

