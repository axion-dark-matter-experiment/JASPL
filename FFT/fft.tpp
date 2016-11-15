template<typename T>
FFT<T>::FFT() {

    static_assert( is_stdlib_container< T >::value, "FFT can only accept container-like objects." );
    static_assert( std::is_floating_point< typename T::value_type >::value, "FFT can only accept floating-point values.");

    /* Setup clFFT. */
    err = clfftInitSetupData(&fftSetup);
    err = clfftSetup(&fftSetup);

    std::string source_directory = SOURCE_DIR;
    std::string kernel_name = "/fft.cl";

    kernel_path = source_directory + kernel_name;

    LoadCLKernel< typename T::value_type >( "PowerSpectrum" );
}

template<typename T>
FFT<T>::~FFT() {
    TearDown();
}

template<typename T>
void FFT<T>::Trigger() {

    /* Execute the plan. */
    err = clfftEnqueueTransform(planHandle, CLFFT_FORWARD, 1, &command_queue(), 0, NULL, NULL, &local_buff(), NULL, NULL);
    std::cout << __func__ << "(clfftEnqueueTransform) OpenCL Status: " << CLErrorToString( err ) << std::endl;

    err = command_queue.enqueueNDRangeKernel( kernel,cl::NullRange, cl::NDRange( signal_size ) );
    signal_size = (signal_size%2 == 0)?( signal_size/2 ):( (signal_size - 1)/2 );
    std::cout << __func__ << "(enqueueNDRangeKernel) OpenCL Status: " << CLErrorToString( err ) << std::endl;

}

template <typename T>
void FFT<T>::SetSignal( cl::Buffer& signal_buff, uint sig_size ) {

    err = kernel.setArg(0, signal_buff);
    std::cout << __func__ << "(setKernelArgs[0]) OpenCL Status: " << CLErrorToString( err ) << std::endl;

    signal_size = sig_size;
    local_buff = signal_buff;

    size_t scratch_bytes = (sig_size%2 == 0)?( sig_size*sizeof(typename T::value_type)/2 ):( (sig_size*sizeof(typename T::value_type) - 1)/2 );
    output_buff = cl::Buffer ( context, CL_MEM_READ_WRITE, scratch_bytes );

    err = kernel.setArg(1, output_buff);
    std::cout << __func__ << "(setKernelArgs[1]) OpenCL Status: " << CLErrorToString( err ) << std::endl;

    size_t clLengths[1] = { sig_size };

    /* Create a default plan for a complex FFT. */
    err = clfftCreateDefaultPlan(&planHandle, context(), dim, clLengths);
    std::cout << __func__ << "(clfftCreateDefaultPlan) OpenCL Status: " << CLErrorToString( err ) << std::endl;

    /* Set plan parameters. */
    if ( boost::is_same< typename T::value_type, float>::value ) {
        std::cout << __func__ << __CLASS__ << "Single Precision" << std::endl;
        err = clfftSetPlanPrecision(planHandle, CLFFT_SINGLE);
    } else {
        std::cout << __func__ << __CLASS__ << "Double Precision" << std::endl;
        err = clfftSetPlanPrecision(planHandle, CLFFT_DOUBLE);
    }

    std::cout << __func__ << "(clfftSetPlanPrecision) OpenCL Status: " << CLErrorToString( err ) << std::endl;
    err = clfftSetLayout(planHandle,  CLFFT_REAL,  CLFFT_HERMITIAN_INTERLEAVED );
    std::cout << __func__ << "(clfftSetLayout) OpenCL Status: " << CLErrorToString( err ) << std::endl;
    err = clfftSetResultLocation(planHandle, CLFFT_INPLACE);
    std::cout << __func__ << "(clfftSetResultLocation) OpenCL Status: " << CLErrorToString( err ) << std::endl;

    /* Bake the plan. */
    err = clfftBakePlan(planHandle, 1, &command_queue(), NULL, NULL);
}

template <typename T>
cl::Buffer& FFT<T>::ProcessedSignal() {
    std::cout << __func__ << "FFT" << std::endl;
    return output_buff;
}

template <typename T>
size_t FFT<T>::ProcessedSignalBytes() {
    std::cout << __func__ << "FFT" << std::endl;
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
