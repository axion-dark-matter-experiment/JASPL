template <typename T> void JFFT::PowerSpectrum( T &input ) {

    check_for_accesor( input );
    check_if_arithmetic( input[0] );

    for ( uint i = 0; i < N ; i++ ) {
        in[i][0] = static_cast<double> (input[i]); //Real Part
        in[i][1] = 0.0; //Imaginary part
    }

    fftw_execute(p);

    for (uint i = 0; i < N ; i++ ) {
        input[i] = pow(out[i][0], 2.0) + pow(out[i][1],2.0);
    }

}
