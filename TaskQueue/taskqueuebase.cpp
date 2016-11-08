#include "TaskQueue/taskqueuebase.h"

namespace jaspl {

namespace ocl {

TaskQueueBase::TaskQueueBase() : OpenCLBase() {
    SetUp( 0 );
    //
}

TaskQueueBase::~TaskQueueBase() {}

void TaskQueueBase::Execute(){

//    for( const auto& kernel : kernel_queue ) {
//        command_queue.enqueueNDRangeKernel( kernel,cl::NullRange, cl::NDRange( signal_size ) );
//    }
    for( auto& task : task_queue ) {
        task.Trigger();
    }

    command_queue.finish();
}

void TaskQueueBase::AddTaskItem( TaskItem& item ){

    item.SetSignal( signal_input, signal_size );
    task_queue.push_back( item );
//    kernel_queue.push_back( item.PullKernel() );

}

}

}
