#ifndef POWERSPECTRUM_H
#define POWERSPECTRUM_H

//Header for this file
//
//C System-Headers
//
//C++ System headers
#include <boost/type_traits.hpp>
//OpenCL Headers
//
//Boost Headers
//
//Project specific headers
#include "../JASPL/TaskItems/FFT/fft.h"
#include "../JASPL/jTypeTraits/jtypetraits.h"

namespace jaspl {

namespace ocl {

template <class T>
class PowerSpectrum : public FFT<T> {

  public:
    PowerSpectrum();

  private:
    void Trigger();
    void SetSignal(cl::Buffer& signal_buff , uint sig_size);

    virtual cl::Buffer& ProcessedSignal();
    virtual size_t ProcessedSignalBytes();
    virtual size_t ProcessedSignalSize();

    cl::Buffer output_buff;
};

#include "TaskItems/PowerSpectrum/powerspectrum.tpp"

}

}

#endif // POWERSPECTRUM_H
