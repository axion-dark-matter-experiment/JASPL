#ifndef SCALARADD_H
#define SCALARADD_H

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
class ScalarAdd : public TaskItem {

  public:

    ScalarAdd( typename T::value_type scalar_value );
    ~ScalarAdd();

  protected:

    void Trigger();
    void SetSignal(cl::Buffer& signal_buff , uint sig_size);

    cl::Buffer& ProcessedSignal();
    size_t ProcessedSignalBytes();
    size_t ProcessedSignalSize();
    bool NeedsToReknew();

    cl_int err;
};

#include "TaskItems/Arithmetic/ScalarAdd/scalaradd.tpp"

}

}

#endif // SCALARADD_H
