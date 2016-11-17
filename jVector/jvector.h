#ifndef JVECTOR_H
#define JVECTOR_H

//C System-Headers
//
//C++ System headers
#include <vector>
#include <string>
#include <iostream>
//Boost Headers
#include <boost/lexical_cast.hpp>  //lexical cast (unsurprisingly)
//Miscellaneous Headers
//
//Project specific headers
#include "jTypeTraits/jtypetraits.h"

namespace jaspl {

template <class F>
class JVector {

  public:
    JVector(std::string raw_data);
    JVector(std::vector<F> vec);
    JVector(F* ptr, uint ptr_size);
    JVector(uint size);
    JVector(uint size, F fill_element);
    JVector();

    ~JVector();

    template <class T> friend void plot ( JVector<T>& vec );
    template <class T> friend void plot ( JVector<T>& vec, std::string plot_title );

    #include "jvector_templates.tpp"

    double norm();
    void Normalize();

    double std_dev();
    double mean();

    F min();
    F max();

    uint size();

  private:
    bool check_if_arithmetic(F input);

    void ParseRawData(std::string raw);
    uint num_chars(std:: string raw_data, char delim );

    double sum( std::vector<F>& data_list , double exponent = 1.0 );
    double mean( std::vector<F>& data_list );
    double std_dev( std::vector<F> &data_list );

    std::vector<F> underlying_vector;
};

}
#endif // JVECTOR_H
