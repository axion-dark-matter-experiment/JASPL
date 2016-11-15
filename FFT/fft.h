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
#include "jVector/jvector.h"
#include "TaskItem/taskitem.h"

namespace jaspl {

namespace ocl {

template <class T>
class FFT : public TaskItem {

//    friend class TaskQueueBase;

  public:
    FFT();
    ~FFT();
//    void Trigger();
//    void SetSignal(cl::Buffer& signal_buff , uint sig_size);

//    cl::Buffer& ProcessedSignal();
//    size_t ProcessedSignalBytes();
//    size_t ProcessedSignalSize();

  private:

    void Trigger();
    void SetSignal(cl::Buffer& signal_buff , uint sig_size);

    cl::Buffer& ProcessedSignal();
    size_t ProcessedSignalBytes();
    size_t ProcessedSignalSize();

    void TearDown();

    cl::Buffer local_buff;
    cl::Buffer output_buff;

    cl_int err;

    clfftPlanHandle planHandle;
    clfftSetupData fftSetup;
    clfftDim dim = CLFFT_1D ;

};

#include "FFT/fft.tpp"

}

}

#endif // FFT_H
