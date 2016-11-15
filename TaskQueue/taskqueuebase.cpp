#include "TaskQueue/taskqueuebase.h"

namespace jaspl {

namespace ocl {

TaskQueueBase::TaskQueueBase() : OpenCLBase() {
//    SetUp( 0 );
    //
}

TaskQueueBase::~TaskQueueBase() {}

void TaskQueueBase::Execute(){

    for( auto& task : task_queue ) {
        task->SetSignal( signal_input, signal_size);
        task->Trigger();
        ReknewSignal( task->ProcessedSignal(),\
                      task->ProcessedSignalBytes(),\
                      task->ProcessedSignalSize() );

        //Wait for commands to finish before iterating, since
        //signal may have been updated
        command_queue.finish();
    }

//    command_queue.finish();
}

void TaskQueueBase::AddTaskItem( TaskItem& item ){

//    item.SetSignal( signal_input, signal_size );
    task_queue.push_back( &item );

}

void TaskQueueBase::ReknewSignal(cl::Buffer& processed_buff, size_t processed_bytes, size_t processed_size ) {

    cl_int* err_ptr = NULL;
    signal_input = cl::Buffer ( context, CL_MEM_READ_WRITE, processed_bytes, err_ptr );
    std::cout << __func__ << "( cl::Buffer (TaskQueueBase) ) OpenCL Status: " << CLErrorToString( err_ptr ) << std::endl;

    cl_int err = command_queue.enqueueCopyBuffer( processed_buff, signal_input, 0, 0, processed_bytes );
    std::cout << __func__ << "(enqueueCopyBuffer (TaskQueueBase) ) OpenCL Status: " << CLErrorToString( err ) << std::endl;

    signal_bytes = processed_bytes;
    signal_size = processed_size;

}

}

}
