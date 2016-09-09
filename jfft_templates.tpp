template <typename T> void JFFT::PowerSpectrum( T &input ) {

    for ( uint i = 0; i < N ; i++ ) {
        double re = static_cast<double> (input[i]);
        double im = 0.0;
        in[i][0] = re;
        in[i][1] = im;
    }

    fftw_execute(p);

    for (uint i = 0; i < N ; i++ ) {
        input[i] = pow(out[i][0], 2.0) + pow(out[i][1],2.0);
    }

}
