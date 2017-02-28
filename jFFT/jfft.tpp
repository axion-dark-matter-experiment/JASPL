template < typename T >
JFFT<T>::JFFT(uint transform_size , bool use_threading) : threading( use_threading ) {
    static_assert( is_stdlib_container<T>::value, "JFFT::Input must be a container-like object.");
    SetUp( transform_size );

}

template < typename T >
JFFT<T>::~JFFT() {
    TearDown();
}

template < typename T >
void JFFT<T>::SetUp( uint size ) {

    N = size;
    fft_size = ( size%2 == 0 )?( size / 2 ):( ( size - 1 ) / 2 );

    norm_factor = static_cast< typename T::value_type >( N );

    //Setup multi-threading
    if ( threading ) {

        int val = fftw_init_threads();

        //if val is equal to zero multithreading cannot be setup
        if( val == 0 ) {
            std::string err_mesg = __func__;
            err_mesg += "Could not setup multithreading.";
            throw std::runtime_error(err_mesg);
        }
        //set plans to use what OpenMP has decided is a good number of
        //threads (usually the number of processor cores)
        fftw_plan_with_nthreads(omp_get_max_threads());

    }

    if( in != NULL ) {
        fftw_free( in );
    }

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    if( out != NULL ) {
        fftw_free( out );
    }

    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    fftw_make_planner_thread_safe();
    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

}

template < typename T >
void JFFT<T>::TearDown() {

    fftw_destroy_plan(p);

    fftw_free(in);
    fftw_free(out);
}

template < typename T >
T JFFT<T>::PowerSpectrum( const T& input ) {

    // get upgradable access
    boost::upgrade_lock<boost::shared_mutex> lock( monitor );
    // get exclusive access
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

    for ( uint i = 0; i < N ; i++ ) {
        in[i][0] = input[i]; //Real Part
        in[i][1] = 0.0; //Imaginary part
    }

    fftw_execute(p);

    T output;
    output.reserve( fft_size );

//    for ( uint i = 0; i < fft_size ; i++ ) {
      for ( uint i = fft_size; i < N ; i++ ) {

          auto val = out[i][0]*out[i][0] + out[i][1]*out[i][1];
          val += out[N-i][0]*out[N-i][0] + out[N-i][1]*out[N-i][1];
          val /= ( norm_factor * norm_factor );
          val *= 2.0;

        //auto val = 2.0*( out[i][0]*out[i][0] + out[i][1]*out[i][1] )/( norm_factor * norm_factor );

        output.push_back( val );

    }

    return output;

}
