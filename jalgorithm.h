#ifndef JALGORITHM_H
#define JALGORITHM_H

// C System-Headers
//
// C++ System headers
#include <type_traits>
#include <string>
#include <assert.h> //static_assert
// Boost Headers
#include <boost/lexical_cast.hpp>
// Miscellaneous Headers
//

namespace jaspl {

template <typename T> void check_if_arithmetic(T& to_check) {
    if ( !std::is_arithmetic<T>::value ) {

        std::string err_mesg = __FUNCTION__;
        err_mesg += "cannot initialize from non-arithmetic type ";
        err_mesg += boost::lexical_cast<std::string>(typeid(to_check).name());
        throw std::invalid_argument(err_mesg);
    }
}

}

#endif // JALGORITHM_H
