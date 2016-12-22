template <typename T> void JFFT::PowerSpectrum( T&input ) {

    static_assert( std::is_pointer<T>::value || is_stdlib_container<T>::value, "JFFT::Input must be pointer of container-like object.");

    for ( uint i = 0; i < N ; i++ ) {
        in[i][0] = static_cast<float> (input[i]); //Real Part
        in[i][1] = 0.0f; //Imaginary part
    }

    fftw_execute(p);

    for ( uint i = 0; i < N ; i++ ) {
        input[i] = out[i][0]*out[i][0] + out[i][1]*out[i][1];
        input[i] /= N_f;
    }

}
