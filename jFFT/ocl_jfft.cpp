//Header for this file
#include "ocl_jfft.h"
//C System-Headers
#include <stdlib.h>
#include <math.h>
//C++ System headers

//OpenCL Headers
#include <CL/cl.hpp>
//Boost Headers

//Project specific headers

namespace jaspl {

namespace ocl {

JFFT::JFFT() {

}

void JFFT::Setup () {

    /* Setup OpenCL environment. */
    err = clGetPlatformIDs( 1, &platform, NULL );
    err = clGetDeviceIDs( platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL );

    props[1] = (cl_context_properties)platform;
    ctx = clCreateContext( props, 1, &device, NULL, NULL, &err );
    queue = clCreateCommandQueue( ctx, device, 0, &err );

    /* Setup clFFT. */
    clfftSetupData fftSetup;
    err = clfftInitSetupData(&fftSetup);
    err = clfftSetup(&fftSetup);

}


void JFFT::Teardown() {
    /* Release the plan. */
    err = clfftDestroyPlan( &planHandle );

    /* Release clFFT library. */
    clfftTeardown();

    /* Release OpenCL working objects. */
    clReleaseCommandQueue( queue );
    clReleaseContext( ctx );

}

}

}
