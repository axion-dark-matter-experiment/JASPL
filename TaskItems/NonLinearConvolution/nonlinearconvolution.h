#ifndef NONLINEARCONVOLUTION_H
#define NONLINEARCONVOLUTION_H

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
class NonLinearConvolution :TaskItem {

  public:
    NonLinearConvolution( T& convolution_kernel );
    NonLinearConvolution( T* convolution_kernel );
    ~NonLinearConvolution();

  private:
    void Trigger();
    void SetSignal(cl::Buffer& signal_buff , uint sig_size);

    cl::Buffer& ProcessedSignal();
    size_t ProcessedSignalBytes();
    size_t ProcessedSignalSize();

    cl::Buffer kernel_buff;
    cl::Buffer scratch_buff;

};

}

}

#include "TaskItems/NonLinearConvolution/nonlinearconvolution.tpp"

#endif // NONLINEARCONVOLUTION_H
