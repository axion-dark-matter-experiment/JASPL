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

    RecurseMean( uint num_samples );

    void operator()( const T&  next_value );

    void Reset();
    T ReturnValue();

  private:

    T last;
    typename T::value_type index = static_cast< typename T::value_type >( 0 );
};


#include "jAlgorithm/jalgorithm.tpp"

}

#endif // JALGORITHM_H
