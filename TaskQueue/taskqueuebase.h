#ifndef TASKQUEUEBASE_H
#define TASKQUEUEBASE_H

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
#include "TaskItem/taskitem.h"
#include "jAlgorithm/jalgorithm.h"

namespace jaspl {

namespace ocl {

class TaskQueueBase : public OpenCLBase {

  public:
    TaskQueueBase();
    virtual ~TaskQueueBase() = 0;
    void Execute();
    void AddTaskItem( TaskItem& item );

  protected:
    std::vector<cl::Kernel> kernel_queue;
    std::vector< TaskItem > task_queue;

    cl::Buffer signal_input;
    cl::Buffer scratch_space;
    cl::Buffer output;

    uint signal_size;
    size_t signal_bytes;

    bool on_device = false;

};

}

}

#endif // TASKQUEUEBASE_H
