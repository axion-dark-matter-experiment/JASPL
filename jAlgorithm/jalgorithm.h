#ifndef JALGORITHM_H
#define JALGORITHM_H


//Header for this file
//
//C System-Headers
//
//C++ System headers
//
//OpenCL Headers
//
//Boost Headers
//
//Project specific headers
#include "jTypeTraits/jtypetraits.h"

namespace jaspl {

template < typename T >
class RecurseMean {

  public:
    RecurseMean( T& initial_value );
    RecurseMean( T initial_value );

    void operator()( T&  next_value );
    void operator()( T  next_value );
    T ReturnValue();

  private:
    T current;
    T last;
//    long unsigned int index = 1;
    double index = 1.0f;
};

#include "jAlgorithm/jalgorithm.tpp"

}

#endif // JALGORITHM_H
