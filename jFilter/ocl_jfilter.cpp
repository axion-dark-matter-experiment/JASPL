#include "ocl_jfilter.h"
#include <boost/filesystem.hpp>

namespace jaspl {

namespace ocl {

JFilter::JFilter( uint device_number ) : OpenCLBase( device_number ) {}

void JFilter::TearDown(){}

std::string FastRead( std::string file_name ) {

    std::ifstream file_stream( file_name.c_str() );
    std::stringstream buffer;
    buffer << file_stream.rdbuf();

    return buffer.str();
}

void JFilter::EstablishKernelPath( std::string kernel_source_path ) {

        std::ifstream test_stream( kernel_source_path.c_str() );
        if ( test_stream.good() ) {
            kernel_path = kernel_source_path;
        } else {
            std::string err_str = __FUNCTION__;
            err_str += "Could not locate OpenCL kernel source file @";
            err_str += kernel_source_path;
            throw std::ios_base::failure( err_str );
        }
}

std::string JFilter::GetOpenCLSource( std::string file_name ) {

    boost::filesystem::path p( __FILE__ );
    boost::filesystem::path dir = p.parent_path();

    std::string kernel_path = dir.string();
    kernel_path += "/";
    kernel_path += file_name;

    std::string kernel_str = FastRead( kernel_path );

    if( kernel_str.empty() ) {
        std::string err_str = __FUNCTION__;
        err_str += "Could not load OpenCL Kernel @";
        err_str += kernel_path;
        throw std::runtime_error( err_str );
    } else {
        return kernel_str;
    }
}

}

}
