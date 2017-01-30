#ifndef LINEARCONVOLUTION_H
#define LINEARCONVOLUTION_H

//Header for this file
//
//C System-Headers
//
//C++ System headers
//
//OpenCL Headers
//
//Boost Headers
//
//Project specific headers
#include "../JASPL/jTypeTraits/jtypetraits.h"
#include "../JASPL/TaskItem/taskitem.h"

namespace jaspl {

namespace ocl {

template <class T>
class LinearConvolution : public TaskItem {


  public:
    LinearConvolution( T& convolution_kernel );
    LinearConvolution( T* convolution_kernel );
    ~LinearConvolution();

  private:
    void Trigger();
    void SetSignal(cl::Buffer& signal_buff , uint sig_size);

    cl::Buffer& ProcessedSignal();
    size_t ProcessedSignalBytes();
    size_t ProcessedSignalSize();
    bool NeedsToReknew();

    cl::Buffer kernel_buff;
    cl::Buffer scratch_buff;

};

#include "TaskItems/LinearConvolution/linearconvolution.tpp"

}

}

#endif // LINEARCONVOLUTION_H
