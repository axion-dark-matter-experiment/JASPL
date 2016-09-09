#ifndef OCL_JFFT_H
#define OCL_JFFT_H

//Header for this file

//C System-Headers

//C++ System headers

//OpenCL Headers
#include <clFFT.h>
//Boost Headers

//Project specific headers
#include "jvector.h"

namespace jaspl {

namespace ocl {

class JFFT {
  public:
    JFFT();
    void ListCLDevices();

    void Setup();
    template <typename T> void PowerSpectrum( T &input );
    void Teardown();

  private:
    cl_int err;
    cl_platform_id platform = 0;
    cl_device_id device = 0;
    cl_context_properties props[3] = { CL_CONTEXT_PLATFORM, 0, 0 };
    cl_context ctx = 0;
    cl_command_queue queue = 0;
    cl_mem bufX;
    float *X;
    cl_event event = NULL;
    int ret = 0;

    clfftPlanHandle planHandle;
    clfftDim dim = CLFFT_1D;
};

#include "ocl_jfft_templates.tpp"
}

}

#endif // OCL_JFFT_H
