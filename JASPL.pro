TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += c++11

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
    jAlgorithm/jalgorithm.cpp \
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
    TaskQueue/taskqueue_templates.tpp

HEADERS += \
    includes/gnuplot-iostream.h \
    jAlgorithm/jalgorithm.h \
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
    TaskQueue/taskqueue.h

DISTFILES += \
    jFilter/jfilter_linearconvolve.cl \
    jFilter/jfilter_nonlinearconvolve.cl \
    jFilter/jfilter_templated_linearconvolve.cl

