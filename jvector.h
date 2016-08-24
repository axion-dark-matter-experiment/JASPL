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

namespace jaspl {

//template <class F> class JVector;
//template <class F> std::ostream& operator<< (std::ostream& stream, JVector<F>& spectrum);

//template <class F> bool operator== (JVector<F>& vector_a, JVector<F>& vector_b);
//template <class F> bool operator!= (JVector<F>& vector_a, JVector<F>& vector_b);

//template <class F> std::ostream& operator<< (std::ostream& stream, JVector<F>& spectrum);
//template <class F> std::ofstream& operator<< (std::ofstream& stream, JVector<F>& spectrum);

//template <class F> JVector<F> operator* (JVector<F>& vector_a, JVector<F>& vector_b);
//template <class F> JVector<F> operator+ (JVector<F>& vector_a, JVector<F>& vector_b);
//template <class F> JVector<F> operator- (JVector<F>& vector_a, JVector<F>& vector_b);

//template <class F> JVector<F> operator* (JVector<F>& vector_a, std::vector<double>& vector_b);
//template <class F> JVector<F> operator+ (JVector<F>& vector_a, std::vector<double>& vector_b);
//template <class F> JVector<F> operator- (JVector<F>& vector_a, std::vector<double>& vector_b);

template <class F>
class JVector {

  public:
    JVector(std::string raw_data);
    JVector(std::vector<F> vec);
//    JVector(uint size);
    ~JVector();

    #include "jvector_templates.tpp"

    double norm();

    double std_dev();
    double mean();

    uint size();
    void push_back();

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
