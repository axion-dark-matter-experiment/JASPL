#ifndef OCL_JFILTER_H
#define OCL_JFILTER_H

//Header for this file
//
//C System-Headers
#include <cxxabi.h> //abi::__cxa_demangle
//C++ System headers
#include <typeinfo> //typeid(T).name()
//OpenCL Headers
#include <CL/cl.h>
#include <CL/cl.hpp>
//Boost Headers
#include <boost/algorithm/string.hpp> //boost::replace_all
//OpenMP Headers
#include<omp.h>
//Project specific headers
#include "OpenCLBase/openclbase.h"
#include "jVector/jvector.h"
#include "jAlgorithm/jalgorithm.h"

namespace jaspl {

namespace ocl {

class JFilter : public OpenCLBase {
  public:
    JFilter( uint device_number = 0 );
    void TearDown();

  protected:
    void EstablishKernelPath( std::string kernel_source_path );
    std::string GetOpenCLSource( std::string file_name );

    template <class T> void LoadCLKernel( T type, std::string kernel_name );

    std::string kernel_path;

    size_t global_size, local_size;
    cl::Program::Sources sources;
    std::string kernel_source;
    cl::Program program;
    cl::Kernel kernel;
    cl::Buffer inputCL;
    cl::Buffer kernelCL;
    cl::Buffer outputCL;

};

#include <jFilter/ocl_jfilter_templates.tpp>

}

}

#endif // OCL_JFILTER_H
