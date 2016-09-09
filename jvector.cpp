// Header for this file
#include "jvector.h"
// C System-Headers
#include <termios.h> /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <fcntl.h>   //fopen(),fclose()
#include <unistd.h>  //read(), write()
#include <stdio.h>

// C++ System headers
#include <vector>      //vector
#include <string>      //string
#include <fstream>     //iss* ofstream
#include <chrono>      // timing functions
#include <cmath>       //sqrt, abs
#include <iostream>    //cout
#include <typeinfo>    //typeid
#include <algorithm>   // transform, find, count, erase
#include <functional>  // plus/minus/multiplies
#include <utility>     //std::make_pair
#include <map>         //std::map
#include <typeinfo>    //typeid
#include <mutex> //protect against concurrent access when using (unordered) parallel for loops
#include <assert.h> //static_assert

// Boost Headers
#include <boost/algorithm/string.hpp>  //split() and is_any_of for parsing .csv files
#include <boost/lexical_cast.hpp>  //lexical cast (unsurprisingly)
#include <dirent.h>

// Miscellaneous Headers
#include <omp.h>  //OpenMP pragmas

namespace jaspl {

template class JVector<int>;
template class JVector<float>;
template class JVector<double>;
template class JVector<char>;

template <class F> JVector<F>::JVector(std::string raw_data) {
    ParseRawData(raw_data);
}

template <class F> JVector<F>::JVector(std::vector<F> vec) {
    underlying_vector = vec;
}

template <class F> JVector<F>::JVector(uint size) {
    underlying_vector.reserve( size );
}

template <class F> JVector<F>::JVector() {}

template <class F> JVector<F>::~JVector() {
    underlying_vector.clear();
}

template <class F> bool JVector<F>::check_if_arithmetic(F input) {

    if ( !std::is_arithmetic<F>::value ) {

        std::string err_mesg = "JVector: ";
        err_mesg += "cannot initialize from non-arithmetic type ";
        err_mesg += boost::lexical_cast<std::string>(typeid(input).name());
        throw std::invalid_argument(err_mesg);

        return false;
    } else {
        return true;
    }
}

template <class F> uint JVector<F>::size() {
    return underlying_vector.size();
}

template <class F> uint JVector<F>::num_chars(std::string raw_data, char delim) {
    return std::count(raw_data.begin(), raw_data.end(), delim);
}

template <class F> void JVector<F>::ParseRawData( std::string raw_data ) {

    uint lines = num_chars(raw_data, '\n');

    std::istringstream data_stream(raw_data);

    for (uint i = 0; i < lines; i++) {
        std::string input;
        std::getline(data_stream, input);
        try {
            F val = boost::lexical_cast<F>(input);
            underlying_vector.push_back(val);
        } catch (const boost::bad_lexical_cast& err) {
            std::cerr << err.what() << std::endl;
        }
    }
}

template <class F> double JVector<F>::sum(std::vector<F>& data_list,double exponent) {

    double tot=0;

    for ( uint i = 0 ; i < data_list.size(); i ++) {
        tot += pow( data_list[i], exponent );
    }

    return tot;
}

template <class F> double JVector<F>::mean(std::vector<F>& data_list) {
    //compute mean value of data set
    double sum_x=sum(data_list,1.0);
    double n=data_list.size();
    return sum_x/n;
}

template <class F> double JVector<F>::mean() {
    return mean(underlying_vector);
}

template <class F> F JVector<F>::min() {
    F min_power= *std::min_element(underlying_vector.begin(), underlying_vector.end());
    return min_power;
}

template <class F> F JVector<F>::max() {
    F min_power= *std::max_element(underlying_vector.begin(), underlying_vector.end());
    return min_power;
}

template <class F> double JVector<F>::std_dev(std::vector<F> &data_list) {

    //compute mean value of data set
    double sum_x=sum(data_list,1.0);
    double n=data_list.size();
    double mean = sum_x/n;

    //compute variance taking into account Bessel's correction i.e. n/(n-1)
    double sum_x2=sum(data_list,2.0);
    double sigma_sqr=sum_x2/(n-1.0)-n/(n-1.0)*pow(mean,2.0);

    //return square root of variance
    return sqrt(sigma_sqr);
}

template <class F> double JVector<F>::std_dev() {
    return std_dev(underlying_vector);
}

template <class F> double JVector<F>::norm() {
    return sqrt(sum(underlying_vector,2.0));

}

template <class F> void JVector<F>::Normalize() {
    double norm_factor=sqrt(sum(underlying_vector,2.0));

    for(unsigned int i = 0; i<underlying_vector.size(); i++) {
        underlying_vector.at(i)=underlying_vector.at(i)/norm_factor;
    }
}

}
