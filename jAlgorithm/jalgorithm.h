#ifndef JALGORITHM_H
#define JALGORITHM_H

// C System-Headers
#include <stdio.h>
// C++ System headers
#include <type_traits>
#include <string>
#include <assert.h> //static_assert
#include <iostream>
#include <fstream>
#include <typeinfo>  // typeid, typeof
#include <cxxabi.h>// abi::__cxa_demangle
//OpenCL Headers
#include <CL/cl.h>
#include <CL/cl.hpp>
// Boost Headers
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
// Miscellaneous Headers
//

#define\
    SOURCE_FILE \
({\
    std::ifstream file_stream(__FILE__);\
    std::stringstream buffer;\
    buffer << file_stream.rdbuf();\
    buffer.str();\
})\

#define\
    SOURCE_DIR \
({\
    std::string current_path = __FILE__;\
    boost::filesystem::path p( current_path );\
    boost::filesystem::path dir = p.parent_path();\
    auto full_path = boost::filesystem::canonical( dir );\
    full_path.string();\
})\

namespace jaspl {

//Get the name of a type as it would appear in source code
template <typename T> std::string get_type_name () {
    int status;
    char* type_name = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
    std::string type_str = std::string( type_name );

    free (type_name);

    return type_str;
}

//Check if given type is a numerical type
template <typename T> void check_if_arithmetic() {
    if ( !std::is_arithmetic<T>::value ) {

        std::string err_mesg = __FUNCTION__;
        err_mesg += "cannot initialize from non-arithmetic type ";
        err_mesg += get_type_name<T>();
        throw std::invalid_argument(err_mesg);
    }
}



// SFINAE test for [] operator
template <typename T>
class has_accessor {
    typedef char one;
    typedef long two;

    template <typename C> static one test( __typeof__(&C::operator[]) ) ;
//    template <typename C> static one test(decltype(&C::operator[])*) ;
    template <typename C> static two test(...);

  public:
    enum { value = sizeof(test<T>(0)) == sizeof(char) };
};

// SFINAE test for data()
template <typename T>
class has_data {
    typedef char one;
    typedef long two;

//    template <typename C> static one test( __typeof__(&C::data) ) ;
    template <typename C> static one test(decltype(&C::data)*) ;
    template <typename C> static two test(...);

  public:
    enum { value = sizeof(test<T>(0)) == sizeof(char) };
};

// SFINAE test for size()
template <typename T>
class has_size {
    typedef char one;
    typedef long two;

//    template <typename C> static one test( __typeof__(&C::size) ) ;
    template <typename C> static one test(decltype(&C::size)*) ;
    template <typename C> static two test(...);

  public:
    enum { value = sizeof(test<T>(0)) == sizeof(char) };
};

//Check for data() method, even if it is overloaded
template <typename T, typename... Args>
class has_data2 {
    template <typename C,typename = decltype( std::declval<C>().data (std::declval<Args>()...) )>
    static std::true_type test(int);
    template <typename C>
    static std::false_type test(...);

  public:
    static constexpr bool value = decltype(test<T>(0))::value;

};

//Part of cxx-prettyprint by Louis Delacroix @https://github.com/louisdx/cxx-prettyprint

template<typename T>
struct has_const_iterator {
  private:
    typedef char                      yes;
    typedef struct {
        char array[2];
    } no;

    template<typename C> static yes test(typename C::const_iterator*);
    template<typename C> static no  test(...);
  public:
    static const bool value = sizeof(test<T>(0)) == sizeof(yes);
    typedef T type;
};

template <typename T>
struct has_begin_end {
    template<typename C> static char (&f(typename std::enable_if<
                                         std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::begin)),
                                         typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

    template<typename C> static char (&f(...))[2];

    template<typename C> static char (&g(typename std::enable_if<
                                         std::is_same<decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::end)),
                                         typename C::const_iterator(C::*)() const>::value, void>::type*))[1];

    template<typename C> static char (&g(...))[2];

    static bool const beg_value = sizeof(f<T>(0)) == 1;
    static bool const end_value = sizeof(g<T>(0)) == 1;
};

//End cxx-prettyprint by Louis Delacroix @https://github.com/louisdx/cxx-prettyprint

template <typename T>
struct is_stdlib_container {

    enum { value = has_const_iterator<T>::value &&\
                   has_begin_end<T>::beg_value &&\
                   has_begin_end<T>::end_value &&\
                   has_size<T>::value
         };
};

//Check a class for the [] operator
template <typename T> void check_for_accesor ( T& to_check ) {

    if( !has_accessor< T >::value ) {
        std::string err_mesg = __FUNCTION__;
        err_mesg += "\nType ";
        err_mesg += get_type_name<T>();
        err_mesg += " lacks accessor operator, []";
        throw std::invalid_argument(err_mesg);
    }
}

namespace ocl {

std::string CLErrorString(cl_int error);
std::string CLErrorString(cl_int* error);
void PrintOCLDebugInfo();

}

}

#endif // JALGORITHM_H
