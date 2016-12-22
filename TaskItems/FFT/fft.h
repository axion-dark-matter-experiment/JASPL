#ifndef FFT_H
#define FFT_H

//Header for this file
//
//C System-Headers
//
//C++ System headers
#include <boost/type_traits.hpp>
//OpenCL Headers
#include <clFFT.h>
//Boost Headers
//
//Project specific headers
#include "../JASPL/jVector/jvector.h"
#include "../JASPL/TaskItem/taskitem.h"

namespace jaspl {

namespace ocl {

template <class T>
class FFT : public TaskItem {

  public:

    FFT();
    ~FFT();

  protected:

    void Trigger();
    virtual void SetSignal(cl::Buffer& signal_buff , uint sig_size);

    virtual cl::Buffer& ProcessedSignal();
    virtual size_t ProcessedSignalBytes();
    virtual size_t ProcessedSignalSize();

    void TearDown();

    cl::Buffer local_buff;

    cl_int err;

    clfftPlanHandle planHandle;
    clfftSetupData fftSetup;
    clfftDim dim = CLFFT_1D ;

};

#include "TaskItems/FFT/fft.tpp"

}

}

#endif // FFT_H
