#include "TaskItem/taskitem.h"

namespace jaspl {

namespace ocl {

//TaskItem::TaskItem( uint device_number ) : OpenCLBase( device_number ) {}

std::string TaskItem::FastRead( std::string file_name ) {

    std::ifstream file_stream( file_name.c_str() );
    std::stringstream buffer;
    buffer << file_stream.rdbuf();
    file_stream.close();

    return buffer.str();
}

void TaskItem::CheckKernelPath( std::string kernel_source_path ) {

    std::ifstream test_stream( kernel_source_path.c_str() );

    if ( !test_stream.good() ) {
        std::string err_str = __func__;
        err_str += "\nCould not locate OpenCL kernel source file :";
        err_str += kernel_source_path;
        throw std::ios_base::failure( err_str );
    } else {
        test_stream.close();
    }
}

std::string TaskItem::GetOpenCLSource( std::string kernel_path ) {

    CheckKernelPath( kernel_path );

    std::string kernel_str = FastRead( kernel_path );

    if( kernel_str.empty() ) {
        std::string err_str = __func__;
        err_str += "\nCould not load OpenCL Kernel :";
        err_str += kernel_path;
        throw std::runtime_error( err_str );
    } else {
        return kernel_str;
    }
}

void TaskItem::Trigger() {

    cl_int err;
    err = command_queue.enqueueNDRangeKernel( kernel,cl::NullRange, cl::NDRange( signal_size ) );

    std::cout << __func__ << " (TaskItem) OpenCL Status: " << CLErrorToString( err ) << std::endl;
}

void TaskItem::SetSignal( cl::Buffer& signal_buff, uint sig_size  ) {

    signal_size = sig_size;

    cl_int err;
    err = kernel.setArg( 0, signal_buff );
    OCL_DEBUG( err );

}

cl::Buffer &TaskItem::ProcessedSignal(){}
size_t TaskItem::ProcessedSignalBytes(){}
size_t TaskItem::ProcessedSignalSize(){}
bool TaskItem::NeedsToReknew() {
    return true;
}

}

}
