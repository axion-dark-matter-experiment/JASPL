#include "ocl_jfilter.h"

#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>//pwd
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fstream>      // std::ifstream
#include<iostream>

namespace jaspl {

namespace ocl {

inline std::string FastRead( std::string file_name ) {
    const char* c_file_name = file_name.c_str();

    std::ifstream file_stream(c_file_name);
    std::stringstream buffer;
    buffer << file_stream.rdbuf();

    return buffer.str();
}

std::string JLinearConvolve::GetOpenCLKernel() {
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    std::string kernel_path = std::string(homedir);
    kernel_path += "/Qt-Projects/JASPL/ocl_jfilter_kernel.cl";

    std::string kernel_str = FastRead( kernel_path );

    if( kernel_str.empty() ) {
        std::string err_str = __FUNCTION__;
        err_str += "Could not load OpenCL Kernel.";
        throw std::runtime_error( err_str );
    } else {
        return kernel_str;
    }
}


void JLinearConvolve::SetUp() {
    //Force kernels to be compiled each time
    setenv("CUDA_CACHE_DISABLE", "1", 1);

    //get all platforms (drivers)
    cl::Platform::get( &all_platforms) ;

    if( all_platforms.size() == 0  ) {
        std::string err_str = __FUNCTION__;
        err_str += "No OpenCL platforms found, check OpenCL installation";
        throw std::runtime_error( err_str );
    }

    default_platform = all_platforms[0];

    default_platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);

    if( all_devices.size() == 0 ) {
        std::string err_str = __FUNCTION__;
        err_str += "No OpenCL GPU devices found, check OpenCL installation";
        throw std::runtime_error( err_str );
    }
    default_device = all_devices[0];

    context = cl::Context ({default_device});

    command_queue = cl::CommandQueue (context,default_device);
}

JLinearConvolve::JLinearConvolve() {

    SetUp();

}

JLinearConvolve::~JLinearConvolve() {}

void JLinearConvolve::TearDown() {
    /* Finalization */

}

}

}
