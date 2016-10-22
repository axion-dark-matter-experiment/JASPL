#ifndef JALGORITHM_H
#define JALGORITHM_H

// C System-Headers
//
// C++ System headers
#include <type_traits>
#include <string>
#include <assert.h> //static_assert
#include <iostream>
#include <typeinfo>  // typeid, typeof
#include <cxxabi.h>// abi::__cxa_demangle
// Boost Headers
#include <boost/lexical_cast.hpp>
// Miscellaneous Headers
//

namespace jaspl {

//Get the name of a type as it would appear in source code
template <typename T> std::string get_type_name (T& type_to_check) {
    int status;
    char* type_name = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
    std::string type_str = std::string( type_name );

    free (type_name);

    return type_str;
}

//Check if given type is a numerical type
template <typename T> void check_if_arithmetic(T& to_check) {
    if ( !std::is_arithmetic<T>::value ) {

        std::string err_mesg = __FUNCTION__;
        err_mesg += "cannot initialize from non-arithmetic type ";
        err_mesg += get_type_name( to_check );
        throw std::invalid_argument(err_mesg);
    }
}

// SFINAE test for [] operator
template <typename T>
class has_accessor {
    typedef char one;
    typedef long two;

    template <typename C> static one test( __typeof__(&C::operator[]) ) ;
    template <typename C> static two test(...);

  public:
    enum { value = sizeof(test<T>(0)) == sizeof(char) };
};

//Check a class for the [] operator
template <typename T> void check_for_accesor ( T& to_check ) {

    if( !has_accessor< T >::value ) {
        std::string err_mesg = __FUNCTION__;
        err_mesg += "\nType ";
        err_mesg += get_type_name( to_check );
        err_mesg += " lacks accessor operator, []";
        throw std::invalid_argument(err_mesg);
    }

}

}

#endif // JALGORITHM_H
