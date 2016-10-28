#include "ocl_jlinearfilter.h"

#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>//pwd
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fstream>      // std::ifstream
#include <iostream>

namespace jaspl {

namespace ocl {

//inline std::string FastRead( std::string file_name ) {
//    const char* c_file_name = file_name.c_str();

//    std::ifstream file_stream(c_file_name);
//    std::stringstream buffer;
//    buffer << file_stream.rdbuf();

//    return buffer.str();
//}

//std::string JLinearConvolve::GetOpenCLKernel() {
//    const char *homedir;
//    if ((homedir = getenv("HOME")) == NULL) {
//        homedir = getpwuid(getuid())->pw_dir;
//    }
//    std::string kernel_path = std::string(homedir);
//    kernel_path += "/Qt-Projects/JASPL/jFilter/jfilter_templated_linearconvolve.cl";

//    std::string kernel_str = FastRead( kernel_path );

//    if( kernel_str.empty() ) {
//        std::string err_str = __FUNCTION__;
//        err_str += "Could not load OpenCL Kernel.";
//        throw std::runtime_error( err_str );
//    } else {
//        return kernel_str;
//    }
//}

JLinearConvolve::JLinearConvolve( uint device_number ) : JFilter( device_number ) {}

}

}
