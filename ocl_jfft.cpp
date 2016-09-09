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

void JFFT::ListCLDevices() {

    //get all platforms (drivers)
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    if(platforms.size()==0) {
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        exit(1);
    }

    cl::Platform default_platform=platforms[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    //get default device of the default platform
    std::vector<cl::Device> devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    if(devices.size()==0) {
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }

    std::cout<<"Number of devices: "<<devices.size()<<std::endl;
    cl::Device default_device=devices[0];
    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<"\n";

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
    clfftTeardown( );

    /* Release OpenCL working objects. */
    clReleaseCommandQueue( queue );
    clReleaseContext( ctx );

}

}

}
