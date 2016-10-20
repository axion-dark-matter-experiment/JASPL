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
#include "jVector/jvector.h"

namespace jaspl {

template <typename T> void plot( std::vector<T> vec ) {

    Gnuplot gp;

    gp << "plot '-' with lines notitle\n";
    gp.send1d(vec);
}

//friend function of JVector
template <class T> void plot ( JVector<T>& vec ){

    Gnuplot gp;

    gp << "plot '-' with lines notitle\n";
    gp.send1d(vec.underlying_vector);
}

//friend function of JVector
template <class T> void plot ( JVector<T>& vec, uint num_plot_points ){

    Gnuplot gp;

    uint pivot = static_cast<uint> (vec.size() / num_plot_points);

    std::vector<uint> pivot_indices;
    for( uint i = 0 ; i < vec.size() ; i ++ ) {
        if ( i%pivot == 0 ) {
            pivot_indices.push_back(i);
        }
    }

    std::vector<double> y_vals;
    for ( const auto& idx : pivot_indices ) {
        y_vals.push_back(vec.at(idx));
    }

    gp << "plot '-' with lines notitle\n";
    gp.send1d(y_vals);
}

//friend function of JVector
template <class T> void plot ( JVector<T>& vec, std::string plot_title ){

    Gnuplot gp;

    gp << "set title '"+plot_title+"'\n";
    gp << "plot '-' with lines notitle\n";
    gp.send1d(vec.underlying_vector);
}


template <class T> void plot ( JVector<T>& vec, std::string plot_title, uint num_plot_points ){

    Gnuplot gp;

    uint pivot = static_cast<uint> (vec.size() / num_plot_points);

    std::vector<uint> pivot_indices;
    for( uint i = 0 ; i < vec.size() ; i ++ ) {
        if ( i%pivot == 0 ) {
            pivot_indices.push_back(i);
        }
    }

    std::vector<double> y_vals;
    for ( const auto& idx : pivot_indices ) {
        y_vals.push_back(vec.at(idx));
    }

    gp << "set title '"+plot_title+"'\n";
    gp << "plot '-' with lines notitle\n";
    gp.send1d(y_vals);
}

}

#endif // JPLOT_H
