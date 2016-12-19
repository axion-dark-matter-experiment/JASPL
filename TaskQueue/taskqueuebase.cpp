//C System-Headers
//
//C++ System headers
#include <algorithm>
//OpenCL Headers
//
//Boost Headers
//
//Qt Headers
//
//Project specific headers
#include "TaskQueue/taskqueuebase.h"

namespace jaspl {

namespace ocl {

TaskQueueBase::TaskQueueBase(uint platform_number, uint device_number) : OpenCLBase( platform_number, device_number ) {}

TaskQueueBase::~TaskQueueBase() {}

void TaskQueueBase::Execute() {

    for( auto& task : task_queue ) {
        task->SetSignal( signal_input, signal_size);
        task->Trigger();

        if ( task->NeedsToReknew() ) {
            ReknewSignal( task->ProcessedSignal(),\
                          task->ProcessedSignalBytes(),\
                          task->ProcessedSignalSize() );
        }

        //Wait for commands to finish before iterating, since
        //signal may have been updated
        command_queue.finish();
    }

//    command_queue.finish();
}

void TaskQueueBase::AddTaskItem( TaskItem& item ) {
    task_queue.push_back( &item );
}

bool TaskQueueBase::RemoveTaskItem(TaskItem &item) {

    if( std::find(task_queue.begin(), task_queue.end(), &item) != task_queue.end() ) {
        task_queue.erase(std::remove(task_queue.begin(), task_queue.end(), &item), task_queue.end());
        return true;
    } else {
        return false;

    }

}

void TaskQueueBase::PrintContents() {

    for( const auto& task : task_queue ) {
        std::cout << get_type_name( task ) << std::endl;
    }

}

void TaskQueueBase::ReknewSignal(cl::Buffer& processed_buff, size_t processed_bytes, size_t processed_size ) {

    cl_int* err_ptr = NULL;
    signal_input = cl::Buffer ( context, CL_MEM_READ_WRITE, processed_bytes, err_ptr );
    OCL_DEBUG( err_ptr );

    cl_int err = command_queue.enqueueCopyBuffer( processed_buff, signal_input, 0, 0, processed_bytes );
    OCL_DEBUG( err );

    signal_bytes = processed_bytes;
    signal_size = processed_size;

}

}

}
