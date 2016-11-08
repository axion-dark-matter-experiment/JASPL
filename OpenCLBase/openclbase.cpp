// Header for this file
#include "openclbase.h"
// C System-Headers
//
// C++ System headers
#include <string>
#include <iostream>
// Boost Headers
#include <boost/lexical_cast.hpp>  //lexical cast (unsurprisingly)
// Miscellaneous Headers
//
//Headers for this project
#include "jAlgorithm/jalgorithm.h"

std::vector<cl::Platform> OpenCLBase::all_platforms;
cl::Platform OpenCLBase::default_platform;
std::vector<cl::Device> OpenCLBase::all_devices;
cl::Device OpenCLBase::current_device;
cl::Context OpenCLBase::context;
cl::CommandQueue OpenCLBase::command_queue;
bool OpenCLBase::initalized = false;

void OpenCLBase::SetUp( uint device_number ) {
    //Force kernels to be compiled each time
    setenv("CUDA_CACHE_DISABLE", "1", 1);

    //get all platforms (drivers)
    cl::Platform::get( &OpenCLBase::all_platforms) ;

    if( OpenCLBase::all_platforms.size() == 0  ) {
        std::string err_str = __FUNCTION__;
        err_str += "No OpenCL platforms found, check OpenCL installation";
        throw std::runtime_error( err_str );
    }

    OpenCLBase::default_platform = OpenCLBase::all_platforms[0];
    OpenCLBase::default_platform.getDevices(CL_DEVICE_TYPE_GPU, &OpenCLBase::all_devices);

    if( OpenCLBase::all_devices.size() == 0 ) {
        std::string err_str = __FUNCTION__;
        err_str += "No OpenCL devices found, check OpenCL installation";
        throw std::runtime_error( err_str );
    }

    if( device_number + 1 > OpenCLBase::all_devices.size() ) {
        std::string err_str = __FUNCTION__;
        err_str += "\nRequested device number of ";
        err_str += boost::lexical_cast<std::string>( device_number );
        err_str += " does not correspond to any device.\n";
        err_str +="Largest device number is ";
        err_str += boost::lexical_cast<std::string>( OpenCLBase::all_devices.size() - 1 );
        throw std::runtime_error( err_str );
    }

    OpenCLBase::current_device = OpenCLBase::all_devices[ device_number ];
    OpenCLBase::context = cl::Context ({OpenCLBase::current_device});

    cl_int err;
    OpenCLBase::command_queue = cl::CommandQueue (OpenCLBase::context,OpenCLBase::current_device, err);
    std::cout << __func__ << " OpenCL Status: " << jaspl::ocl::CLErrorString( err ) << std::endl;
}

OpenCLBase::OpenCLBase( uint device_number ) {

    std::cout << __func__ << "Constructor called: " << initalized << std::endl;

    //Static variables are initalized once and only once
    //these variables need to be consistent across all derived classes
    if( initalized != true ) {
        SetUp( device_number );
    }

    initalized |= true;
}

OpenCLBase::~OpenCLBase() {}

void OpenCLBase::PrintDebugInfo() {
    //get all platforms (drivers)
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    if(platforms.size()==0) {
        std::cout<<" No platforms found. Check OpenCL installation." << std::endl;
    }

    std::cout << "Available Platforms:" << std::endl;
    for( uint i = 0 ; i < platforms.size() ; i++ ) {

        std::cout << "Platform number " << i << " " << platforms[i].getInfo<CL_PLATFORM_NAME>() << std::endl;
    }

    cl::Platform default_platform = platforms[0];
    std::cout << "Using platform: "<<default_platform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    //get default device of the default platform
    std::vector<cl::Device> devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    if(devices.size()==0) {
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }

    std::cout<<"Available devices: "<<devices.size()<<std::endl;

    for( uint i = 0 ; i < devices.size() ; i++ ) {

        std::cout << "Device number " << i << " " << devices[i].getInfo<CL_DEVICE_NAME>() << std::endl;

    }

    cl::Device default_device=devices[0];
    std::cout<< "Using device: "<<default_device.getInfo<CL_DEVICE_NAME>()<<std::endl;
}
