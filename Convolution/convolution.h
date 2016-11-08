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
#include "jVector/jvector.h"
#include "TaskItem/taskitem.h"

namespace jaspl {

namespace ocl {

template <class T>
class Convolution : public TaskItem {

  public:
    Convolution( T& convolution_kernel );
    void SetSignal(cl::Buffer& signal_buff , uint sig_size);

  private:
    cl::Buffer kernel_buff;

};

#include "Convolution/convolution.tpp"

}

}

#endif // CONVOLUTION_H
