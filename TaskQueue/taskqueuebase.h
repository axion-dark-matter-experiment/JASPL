#ifndef TASKQUEUEBASE_H
#define TASKQUEUEBASE_H

//Header for this file
//
//C System-Headers
//
//C++ System headers
#include <iostream>
#include <memory>
//OpenCL Headers
#include <CL/cl.h>
#include <CL/cl.hpp>
//Boost Headers
//
//OpenMP Headers
//
//Project specific headers
#include "OpenCLBase/openclbase.h"
#include "TaskItem/taskitem.h"
#include "jTypeTraits/jtypetraits.h"

namespace jaspl {

namespace ocl {

class TaskQueueBase : public OpenCLBase {

    friend class TaskItem;

  public:
    TaskQueueBase( uint device_number = 0 );
    virtual ~TaskQueueBase() = 0;
    void Execute();
    void AddTaskItem(TaskItem &item );
    void ReknewSignal(cl::Buffer& processed_buff , size_t processed_bytes, size_t processed_size);

  protected:
    std::list< TaskItem* > task_queue;

    cl::Buffer signal_input;
    cl::Buffer output;

    size_t signal_size;
    size_t signal_bytes;

    bool on_device = false;

};

}

}

#endif // TASKQUEUEBASE_H
