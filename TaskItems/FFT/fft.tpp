template<typename T>
FFT<T>::FFT() {

    static_assert( is_stdlib_container< T >::value, "FFT object must be built with container-like class." );
    static_assert( std::is_floating_point< typename T::value_type >::value, "FFT object must be built with floating-point template parameter (e.g. double, float).");

    /* Setup clFFT. */
    err = clfftInitSetupData(&fftSetup);
    err = clfftSetup(&fftSetup);
}

template<typename T>
FFT<T>::~FFT() {
    TearDown();
}

template<typename T>
void FFT<T>::Trigger() {

    /* Execute the plan. */
    err = clfftEnqueueTransform(planHandle, CLFFT_FORWARD, 1, &command_queue(), 0, NULL, NULL, &local_buff(), NULL, NULL);
    OCL_DEBUG( err );

}

template <typename T>
void FFT<T>::SetSignal( cl::Buffer& signal_buff, uint sig_size ) {

    signal_size = sig_size;
    local_buff = signal_buff;

    size_t clLengths[1] = { sig_size };

    /* Create a default plan for a real FFT. */
    err = clfftCreateDefaultPlan(&planHandle, context(), dim, clLengths);
    OCL_DEBUG( err );

    /* Set plan parameters. */
    if ( boost::is_same< typename T::value_type, float>::value ) {
        err = clfftSetPlanPrecision(planHandle, CLFFT_SINGLE);
    } else {
        err = clfftSetPlanPrecision(planHandle, CLFFT_DOUBLE);
    }

    OCL_DEBUG( err );

    err = clfftSetLayout(planHandle,  CLFFT_REAL, CLFFT_HERMITIAN_INTERLEAVED );
    OCL_DEBUG( err );

    err = clfftSetResultLocation(planHandle, CLFFT_INPLACE);
    OCL_DEBUG( err );

    /* Bake the plan. */
    err = clfftBakePlan(planHandle, 1, &command_queue(), NULL, NULL);
}

template <typename T>
cl::Buffer& FFT<T>::ProcessedSignal() {
    return local_buff;
}

template <typename T>
size_t FFT<T>::ProcessedSignalBytes() {
    return signal_size*sizeof( typename T::value_type );
}

template <typename T>
size_t FFT<T>::ProcessedSignalSize() {
    return signal_size;
}


template <typename T>
void FFT<T>::TearDown() {
    /* Release the plan. */
    err = clfftDestroyPlan( &planHandle );
    /* Release clFFT library. */
    clfftTeardown();
}
