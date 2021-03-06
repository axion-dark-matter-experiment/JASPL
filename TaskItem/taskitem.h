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
#include "../JASPL/OpenCLBase/openclbase.h"
#include "../JASPL/jVector/jvector.h"
#include "../JASPL/jTypeTraits/jtypetraits.h"

namespace jaspl {

namespace ocl {

class TaskItem : public OpenCLBase {

    friend class TaskQueueBase;

  public:
    virtual ~TaskItem() {}

  protected:
    template <typename F> std::string FakeKernelTemplating( std::string kernel_source );
    template <typename F> void LoadCLKernel( std::string kernel_name, std::string kernel_src );

    virtual void Trigger();
    virtual void SetSignal( cl::Buffer& signal_buff , uint sig_size );
    virtual cl::Buffer& ProcessedSignal() = 0;
    virtual size_t ProcessedSignalBytes() = 0;
    virtual size_t ProcessedSignalSize() = 0;
    virtual bool NeedsToReknew() = 0;

    void CheckKernelPath( std::string kernel_source_path );
    std::string GetOpenCLSource(std::string kernel_path );

    std::string kernel_path;
    cl::Program::Sources sources;
    std::string kernel_source;
    cl::Program program;
    cl::Kernel kernel;

    size_t signal_size;

  private:
    std::string FastRead( std::string file_name );

};

#include <TaskItem/taskitem_templates.tpp>

}

}

#endif // TASKITEM_H
