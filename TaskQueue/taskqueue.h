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
#include "TaskQueue/taskqueuebase.h"
#include "jTypeTraits/jtypetraits.h"

namespace jaspl {

namespace ocl {

template <typename T>
class TaskQueue : public TaskQueueBase {

  public:
    TaskQueue();
    ~TaskQueue();

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
