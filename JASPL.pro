TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

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
    jvector.cpp \
    jvector_templates.tpp \
    jplot.cpp \
    jfilter.cpp \
    ocl_jfft.cpp \
    ocl_jfft_templates.tpp \
    jfft.cpp \
    jfft_templates.tpp \
    ocl_jfilter.cpp \
    ocl_jfilter_templates.tpp

HEADERS += \
    jvector.h \
    jplot.h \
    jfilter.h \
    ocl_jfft.h \
    jfft.h \
    ocl_jfilter.h

DISTFILES += \
    ocl_jfilter_kernel.cl

