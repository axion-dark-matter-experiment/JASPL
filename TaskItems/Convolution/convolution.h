#ifndef CONVOLUTION_H
#define CONVOLUTION_H

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
#include "jTypeTraits/jtypetraits.h"
#include "TaskItem/taskitem.h"

namespace jaspl {

namespace ocl {

template <class T>
class Convolution : public TaskItem {


  public:
    Convolution( T& convolution_kernel );
    Convolution( T* convolution_kernel );
    ~Convolution();

  private:
    void Trigger();
    void SetSignal(cl::Buffer& signal_buff , uint sig_size);

    cl::Buffer& ProcessedSignal();
    size_t ProcessedSignalBytes();
    size_t ProcessedSignalSize();

    cl::Buffer kernel_buff;
    cl::Buffer scratch_buff;

};

#include "TaskItems/Convolution/convolution.tpp"

}

}

#endif // CONVOLUTION_H
