#ifndef TASKQUEUE_H
#define TASKQUEUE_H

//Header for this file
//
//C System-Headers
//
//C++ System headers
#include <iostream>
//OpenCL Headers
#include <CL/cl.h>
#include <CL/cl.hpp>
//Boost Headers
//
//OpenMP Headers
//
//Project specific headers
#include "OpenCLBase/openclbase.h"
#include "jAlgorithm/jalgorithm.h"

namespace jaspl {

namespace ocl {

class TaskQueueBase : public OpenCLBase {

  protected:
    cl::Buffer signal_input;
    cl::Buffer output;

    uint signal_size;
    size_t signal_bytes;

    bool on_device = false;

};

template <typename T>
class TaskQueue : public TaskQueueBase {

  public:
    TaskQueue();

    void Load(T signal);
    T Recall();
};

template< class T>
class TaskQueue<T*>: public TaskQueueBase {

  public:

    TaskQueue();

    void Load( T* signal_ptr );
    T* Recall();

};

#include <TaskQueue/taskqueue_templates.tpp>

}

}

#endif // TASKQUEUE_H
