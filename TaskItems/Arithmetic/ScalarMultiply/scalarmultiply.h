#ifndef SCALARMULTIPLY_H
#define SCALARMULTIPLY_H

//C System-Headers
//
//C++ System headers
#include <boost/type_traits.hpp>
//OpenCL Headers
//
//Boost Headers
//
//Project specific headers
#include "TaskItem/taskitem.h"

namespace jaspl {

namespace ocl {

template <class T>
class ScalarMultiply : public TaskItem {

  public:

    ScalarMultiply( typename T::value_type scalar_value );
    ~ScalarMultiply();

  protected:

    void Trigger();
    void SetSignal(cl::Buffer& signal_buff , uint sig_size);

    cl::Buffer& ProcessedSignal();
    size_t ProcessedSignalBytes();
    size_t ProcessedSignalSize();
    bool NeedsToReknew();

    cl_int err;
};

#include "TaskItems/Arithmetic/ScalarMultiply/scalarmultiply.tpp"

}

}

#endif // SCALARMULTIPLY_H
