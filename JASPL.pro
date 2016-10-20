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
    jFilter/jfilter.cpp \
    jFilter/ocl_jfilter.cpp \
    jFilter/ocl_jfilter_templates.tpp \
    jPlot/jplot.cpp \
    jVector/jvector.cpp \
    jVector/jvector_templates.tpp

HEADERS += \
    includes/gnuplot-iostream.h \
    jAlgorithm/jalgorithm.h \
    jFFT/jfft.h \
    jFFT/ocl_jfft.h \
    jFilter/jfilter.h \
    jFilter/ocl_jfilter.h \
    jPlot/jplot.h \
    jVector/jvector.h \
    jFilter/jfilter_unit_test.h

DISTFILES += \
    docs/html/search/all_0.js \
    docs/html/search/classes_0.js \
    docs/html/search/search.js \
    docs/html/search/searchdata.js \
    docs/html/dynsections.js \
    docs/html/jquery.js \
    docs/html/search/close.png \
    docs/html/search/mag_sel.png \
    docs/html/search/search_l.png \
    docs/html/search/search_m.png \
    docs/html/search/search_r.png \
    docs/html/arrowdown.png \
    docs/html/arrowright.png \
    docs/html/bc_s.png \
    docs/html/bdwn.png \
    docs/html/closed.png \
    docs/html/doc.png \
    docs/html/doxygen.png \
    docs/html/folderclosed.png \
    docs/html/folderopen.png \
    docs/html/nav_f.png \
    docs/html/nav_g.png \
    docs/html/nav_h.png \
    docs/html/open.png \
    docs/html/splitbar.png \
    docs/html/sync_off.png \
    docs/html/sync_on.png \
    docs/html/tab_a.png \
    docs/html/tab_b.png \
    docs/html/tab_h.png \
    docs/html/tab_s.png \
    docs/html/search/search.css \
    docs/html/doxygen.css \
    docs/html/tabs.css \
    docs/html/search/all_0.html \
    docs/html/search/classes_0.html \
    docs/html/search/nomatches.html \
    docs/html/annotated.html \
    docs/html/classes.html \
    docs/html/classjaspl_1_1_j_f_f_t-members.html \
    docs/html/classjaspl_1_1_j_f_f_t.html \
    docs/html/classjaspl_1_1_j_filter-members.html \
    docs/html/classjaspl_1_1_j_filter.html \
    docs/html/classjaspl_1_1_j_vector-members.html \
    docs/html/classjaspl_1_1_j_vector.html \
    docs/html/classjaspl_1_1ocl_1_1_j_f_f_t-members.html \
    docs/html/classjaspl_1_1ocl_1_1_j_f_f_t.html \
    docs/html/classjaspl_1_1ocl_1_1_j_linear_convolve-members.html \
    docs/html/classjaspl_1_1ocl_1_1_j_linear_convolve.html \
    docs/html/files.html \
    docs/html/graph_legend.html \
    docs/html/index.html \
    docs/html/jalgorithm_8cpp_source.html \
    docs/html/jalgorithm_8h_source.html \
    docs/html/jfft_8cpp_source.html \
    docs/html/jfft_8h_source.html \
    docs/html/jfft__templates_8tpp_source.html \
    docs/html/jfilter_8cpp_source.html \
    docs/html/jfilter_8h_source.html \
    docs/html/jplot_8cpp_source.html \
    docs/html/jplot_8h_source.html \
    docs/html/jvector_8cpp_source.html \
    docs/html/jvector_8h_source.html \
    docs/html/jvector__templates_8tpp_source.html \
    docs/html/main_8cpp_source.html \
    docs/html/ocl__jfft_8cpp_source.html \
    docs/html/ocl__jfft_8h_source.html \
    docs/html/ocl__jfft__templates_8tpp_source.html \
    docs/html/ocl__jfilter_8cpp_source.html \
    docs/html/ocl__jfilter_8h_source.html \
    docs/html/ocl__jfilter__templates_8tpp_source.html \
    docs/html/graph_legend.md5 \
    docs/Doxyfile \
    docs/html/graph_legend.dot \
    docs/latex/annotated.tex \
    docs/latex/classjaspl_1_1_j_f_f_t.tex \
    docs/latex/classjaspl_1_1_j_filter.tex \
    docs/latex/classjaspl_1_1_j_vector.tex \
    docs/latex/classjaspl_1_1ocl_1_1_j_f_f_t.tex \
    docs/latex/classjaspl_1_1ocl_1_1_j_linear_convolve.tex \
    docs/latex/doxygen.sty \
    docs/latex/refman.tex \
    jFilter/jfilter_linearconvolve.cl \
    jFilter/jfilter_nonlinearconvolve.cl \
    jFilter/jfilter_templated_linearconvolve.cl

