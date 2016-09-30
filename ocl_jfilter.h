#ifndef OCL_JFITTER_H
#define OCL_JFITTER_H

#include "jvector.h"
#include "jalgorithm.h"

#include <CL/cl.h>
#include <CL/cl.hpp>

#include <boost/algorithm/string.hpp> //boost::replace_all
#include <typeinfo> //typeid(T).name()
#include <cxxabi.h> //abi::__cxa_demangle

namespace jaspl {

namespace ocl {

class JLinearConvolve {

  public:
    JLinearConvolve();
    ~JLinearConvolve();
    template <class T> JVector<T> Convolve(JVector<T>& signal, JVector<T>& kernel);

    void TearDown();

  private:

    std::string GetOpenCLKernel();
    template <class T> void LoadCLKernel(T type);
    void SetUp();

    size_t global_size, local_size;

    std::vector<cl::Platform> all_platforms;
    cl::Platform default_platform;
    std::vector<cl::Device> all_devices;
    cl::Device default_device;
    cl::Context context;
    cl::Program::Sources sources;
    std::string kernel_source;
    cl::Program program;
    cl::Kernel kernel;
    cl::CommandQueue command_queue;

    cl::Buffer inputCL;
    cl::Buffer kernelCL;
    cl::Buffer outputCL;

};

#include <ocl_jfilter_templates.tpp>

}

}

#endif // OCL_JFITTER_H
