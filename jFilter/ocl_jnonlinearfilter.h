#ifndef JNONLINEARFILTER_H
#define JNONLINEARFILTER_H

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

class JNonLinearFilter : OpenCLBase {

  public:

    JNonLinearFilter( uint device_number = 0 );
};

#endif // JNONLINEARFILTER_H
