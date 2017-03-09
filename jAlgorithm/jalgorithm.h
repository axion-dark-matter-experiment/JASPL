#ifndef JALGORITHM_H
#define JALGORITHM_H

//C System-Headers
//
//C++ System headers
#include <mutex>
//OpenCL Headers
//
// Boost Headers
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/lexical_cast.hpp>
//Project specific headers
#include "../JASPL/jTypeTraits/jtypetraits.h"

namespace jaspl {

template < typename T >
/*!
 * \brief Average together a (nearly) arbitrary number of container-like objects
 * using the recursive definition of the expected value defined by:
 *
 * \f[
 *      \mu_i = \frac{ i - 1 }{ i } \mu_{ i - 1 } + \frac{1}{i} x_i\\
 *      \text{ for } x_i \in \mathbb{R}^n \ \mu_i \in \mathcal{R} \\
 *      \text{ and running index } i \in \mathbb{N}
 * \f]
 *
 * Template parameter needs to be a container-liker object filled with floating-point values.
 */
class RecurseMean {

  public:

    RecurseMean( uint num_samples );

    /*!
     * \brief Average together a new value
     *
     * \param next_value
     *
     * The container to be averaged
     */
    void operator()( const T&  next_value );

    void Reset();
    typename T::value_type Index();

    T ReturnValue();

  private:

    T last;
    typename T::value_type index;
    boost::shared_mutex monitor;
};


#include "../JASPL/jAlgorithm/jalgorithm.tpp"

}

#endif // JALGORITHM_H
