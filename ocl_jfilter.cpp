#include "ocl_jfilter.h"

#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>//pwd
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <fstream>      // std::ifstream
#include<iostream>

#define MEM_SIZE (256)
#define MAX_SOURCE_SIZE (0x100000)

namespace jaspl {

namespace ocl {

inline std::string FastRead( std::string file_name ) {
    const char* c_file_name = file_name.c_str();

    std::ifstream file_stream(c_file_name);
    std::stringstream buffer;
    buffer << file_stream.rdbuf();

    return buffer.str();
}

void JLinearConvolve::LoadCLKernel() {

    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }
    std::string kernel_path = std::string(homedir);
    kernel_path += "/Qt-Projects/JASPL/ocl_jfilter_kernel.cl";

    std::string kernel_str = FastRead( kernel_path );

    if( kernel_str.empty() ) {
        std::cerr << "Failed to load kernel." << std::endl;
        return;
    }

    source_str = new char[kernel_str.size() + 1];
    std::copy(kernel_str.begin(), kernel_str.end(), source_str);
    source_str[kernel_str.size()] = '\0';

}

void JLinearConvolve::SetUp() {

    //Force kernels to be compiled each time
    setenv("CUDA_CACHE_DISABLE", "1", 1);
    putenv("CUDA_CACHE_DISABLE=1");
    /* Get Platform and Device Info */
    ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

    /* Create OpenCL context */
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

    /* Create Command Queue */
    command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    /* Create Kernel Program from the source */
    program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);

    /* Build Kernel Program */
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    //Check to make sure kernel compiled successfully.
    size_t len = 0;

    ret = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &len);

    char *buffer = (char*)malloc(len*sizeof(char));
    ret = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, len, buffer, NULL);

    std::cout << buffer << std::endl;

    free (buffer);

    /* Create OpenCL Kernel */
    kernel = clCreateKernel(program, "Convolve", &ret);
}

JLinearConvolve::JLinearConvolve() {

    LoadCLKernel();
    SetUp();
}

JLinearConvolve::~JLinearConvolve() {}

void JLinearConvolve::TearDown() {
    /* Finalization */
//    ret = clFlush(command_queue);

    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);

    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);

    free(source_str);
}

}

}
