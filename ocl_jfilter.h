#ifndef OCL_JFITTER_H
#define OCL_JFITTER_H

#include "jvector.h"

#include <CL/cl.h>
#include <CL/cl.hpp>

namespace jaspl {

namespace ocl {

class JLinearConvolve {

  public:
    JLinearConvolve();
    ~JLinearConvolve();

    template <class T> void SetKernel( JVector<T>& conv_kernel );
    template <class T> JVector<T> Convolve( JVector<T>& signal );

    void TearDown();

  private:

    void LoadCLKernel();
    void SetUp();

    float* convolution_kernel;
    int convolution_kernel_size;

    char* source_str;
    size_t source_size;

    cl_device_id device_id = NULL;
    cl_context context = NULL;
    cl_command_queue command_queue = NULL;

    // Device input buffers
    cl_mem inputCL = NULL;
    cl_mem kernelCL = NULL;
    // Device output buffer
    cl_mem outputCL = NULL;

    cl_program program = NULL;
    cl_kernel kernel = NULL;
    cl_platform_id platform_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret;

    size_t global_size, local_size;

};

#include <ocl_jfilter_templates.tpp>

}

}

#endif // OCL_JFITTER_H
