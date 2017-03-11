template < typename T >
JFFT<T>::JFFT( bool use_threading ) : threading( use_threading ) {
    static_assert( is_stdlib_container<T>::value, "JFFT::Input must be a container-like object.");
}

template < typename T >
JFFT<T>::~JFFT() {
    TearDown();
}

template < typename T >
void JFFT<T>::SetUp( uint size ) {

    if( set_up ) {
        return;
    }

    set_up = true;

    N = size;
    fft_size = ( size%2 == 0 )?( size / 2 ):( ( size - 1 ) / 2 );

    norm_factor = static_cast< typename T::value_type >( N );

    //Setup multi-threading
    if ( threading ) {

        int val = fftwf_init_threads();

        //if val is equal to zero multithreading cannot be setup
        if( val == 0 ) {
            std::string err_mesg = __func__;
            err_mesg += "Could not setup multithreading.";
            throw std::runtime_error(err_mesg);
        }
        //set plans to use what OpenMP has decided is a good number of
        //threads (usually the number of processor cores)
        fftwf_plan_with_nthreads(omp_get_max_threads());

    }

    if( in != NULL ) {
        fftwf_free( in );
        std::string err_mesg = __func__;
        err_mesg += "Could not allocate memory for FFT input buffer.";
        throw std::runtime_error(err_mesg);
    }

    in = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * N);

    if( out != NULL ) {
        fftwf_free( out );
        std::string err_mesg = __func__;
        err_mesg += "Could not allocate memory for FFT output buffer.";
        throw std::runtime_error(err_mesg);
    }

    out = (fftwf_complex*) fftwf_malloc(sizeof(fftwf_complex) * N);

    fftwf_make_planner_thread_safe();
    p = fftwf_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

}

template < typename T >
void JFFT<T>::TearDown() {

    fftwf_destroy_plan(p);

    fftwf_free(in);
    fftwf_free(out);
}

inline double abs_sqr( const fftwf_complex& z ) {
    return z[0]*z[0] + z[1]*z[1];
}

template < typename T >
T JFFT<T>::PowerSpectrum( const T& input ) {

    if( set_up == false ) {
        std::string err_mesg = __func__;
        err_mesg += "JFFT has not been set-up properly --";
        err_mesg += " you must call JFFT::SetUp( uint size ) first.";
        throw std::runtime_error(err_mesg);
    }

    // get upgradable access
    boost::upgrade_lock<boost::shared_mutex> lock( monitor );
    // get exclusive access
    boost::upgrade_to_unique_lock<boost::shared_mutex> uniqueLock(lock);

    std::cout << "N " << N << std::endl;

    for ( uint i = 0; i < N ; i++ ) {
        in[i][0] = input.at(i); //Real Part
        in[i][1] = 0.0; //Imaginary part
    }

    fftwf_execute( p );

    T output;
    output.reserve( fft_size );

    auto N_sqr = norm_factor * norm_factor;

    // Explicitly compute first element
    auto C_0 = abs_sqr( out[0] );
    C_0 /= N_sqr;
//    C_0 *= 2.0;

    output.push_back( C_0 );

    for ( uint i = 1; i < fft_size - 1; i++ ) {

        auto C_i = abs_sqr( out[i] ) + abs_sqr( out[N-i] );;
        C_i /= N_sqr;
//        C_i *= 2.0;

        output.push_back( C_i );

    }

    // Explicitly compute last element
    auto C_N_half = abs_sqr( out[ fft_size - 1 ] );
    C_N_half /= N_sqr;
//    C_N_half *= 2.0;

    output.push_back( C_N_half );

    return output;

}
