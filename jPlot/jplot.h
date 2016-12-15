#ifndef JPLOT_H
#define JPLOT_H

//Header for this file

//C System-Headers

//C++ System headers

//OpenCL Headers

//Boost Headers

//GnuPlot Headers
#include "includes/gnuplot-iostream.h"
//Project specific headers
#include "jTypeTraits/jtypetraits.h"

namespace jaspl {

template <typename T> void plot( T signal ) {

    static_assert(is_stdlib_container< T >::value,
                  "Can only plot container-like objects.");
    static_assert(std::is_arithmetic< typename T::value_type >::value,
                  "Can only plot arithmetic types.");

    Gnuplot gp;

    gp << "plot '-' with lines notitle\n";
    gp.send1d(signal);
}

template <class T> void plot ( T signal, uint num_plot_points ) {

    static_assert(is_stdlib_container< T >::value,
                  "Can only plot container-like objects.");
    static_assert(std::is_arithmetic< typename T::value_type >::value,
                  "Can only plot arithmetic types.");

    Gnuplot gp;

    uint pivot = static_cast<uint> (signal.size() / num_plot_points);

    std::vector<uint> pivot_indices;

    for( uint i = 0 ; i < signal.size() ; i ++ ) {
        if ( i%pivot == 0 ) {
            pivot_indices.push_back(i);
        }
    }

    T y_vals;

    for ( const auto& idx : pivot_indices ) {
        y_vals.push_back( signal.at(idx) );
    }

    gp << "plot '-' with lines notitle\n";
    gp.send1d(y_vals);
}


//friend function of JVector
template <class T> void plot ( T& signal, std::string plot_title ) {

    static_assert(is_stdlib_container< T >::value,
                  "Can only plot container-like objects.");
    static_assert(std::is_arithmetic< typename T::value_type >::value,
                  "Can only plot arithmetic types.");

    Gnuplot gp;

    gp << "set title '"+plot_title+"'\n";
    gp << "plot '-' with lines notitle\n";
    gp.send1d(signal);
}


template <class T> void plot ( T& signal, std::string plot_title, uint num_plot_points ) {

    static_assert(is_stdlib_container< T >::value,
                  "Can only plot container-like objects.");
    static_assert(std::is_arithmetic< typename T::value_type >::value,
                  "Can only plot arithmetic types.");

    Gnuplot gp;

    uint pivot = static_cast<uint> (signal.size() / num_plot_points);

    std::vector<uint> pivot_indices;

    for( uint i = 0 ; i < signal.size() ; i ++ ) {
        if ( i%pivot == 0 ) {
            pivot_indices.push_back(i);
        }
    }

    T y_vals;

    for ( const auto& idx : pivot_indices ) {
        y_vals.push_back( signal.at(idx) );
    }

    gp << "set title '"+plot_title+"'\n";
    gp << "plot '-' with lines notitle\n";
    gp.send1d(y_vals);
}

}

#endif // JPLOT_H
