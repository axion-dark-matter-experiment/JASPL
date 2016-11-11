#ifndef TASKITEM_H
#define TASKITEM_H

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

class TaskItem : public OpenCLBase {

  public:

//    TaskItem( uint device_number = 0 );
    virtual void Trigger();
    virtual void SetSignal(cl::Buffer& signal_buff , uint sig_size);

  protected:
    template <typename T> std::string FakeKernelTemplating( std::string kernel_source );
    template <typename T> void LoadCLKernel( std::string kernel_name );

    void CheckKernelPath( std::string kernel_source_path );
    std::string GetOpenCLSource(std::string kernel_path );

    std::string kernel_path;
    cl::Program::Sources sources;
    std::string kernel_source;
    cl::Program program;
    cl::Kernel kernel;

    uint signal_size;

  private:
    std::string FastRead(std::__cxx11::string file_name );

};

#include <TaskItem/taskitem_templates.tpp>

}

}

#endif // TASKITEM_H
