template<typename T>
PowerSpectrum<T>::PowerSpectrum() {

    std::string source_directory = SOURCE_DIR;
    std::string kernel_name = "/powerspectrum.cl";

    FFT<T>::kernel_path = source_directory + kernel_name;

    TaskItem::LoadCLKernel< typename T::value_type >( "PowerSpectrum" );
}

template<typename T>
void PowerSpectrum<T>::Trigger() {

    FFT<T>::Trigger();

    FFT<T>::err = TaskItem::command_queue.enqueueNDRangeKernel( TaskItem::kernel,cl::NullRange, cl::NDRange( TaskItem::signal_size ) );
    TaskItem::signal_size = (TaskItem::signal_size%2 == 0)?( TaskItem::signal_size/2 ):( (TaskItem::signal_size - 1)/2 );
    std::cout << __func__ << "(enqueueNDRangeKernel) OpenCL Status: " << CLErrorToString( FFT<T>::err ) << std::endl;

}

template <typename T>
void PowerSpectrum<T>::SetSignal( cl::Buffer& signal_buff, uint sig_size ) {

    FFT<T>::err = TaskItem::kernel.setArg(0, signal_buff);
    std::cout << __func__ << "(setKernelArgs[0]) OpenCL Status: " << CLErrorToString( FFT<T>::err ) << std::endl;

    TaskItem::signal_size = sig_size;
    FFT<T>::local_buff = signal_buff;

    size_t scratch_bytes = (sig_size%2 == 0)?( sig_size*sizeof(typename T::value_type)/2 ):( (sig_size*sizeof(typename T::value_type) - 1)/2 );
    output_buff = cl::Buffer ( OpenCLBase::context, CL_MEM_READ_WRITE, scratch_bytes );

    FFT<T>::err = TaskItem::kernel.setArg(1, output_buff);
    std::cout << __func__ << "(setKernelArgs[1]) OpenCL Status: " << CLErrorToString(  FFT<T>::err ) << std::endl;

    size_t clLengths[1] = { sig_size };

    /* Create a default plan for a complex FFT. */
    FFT<T>::err = clfftCreateDefaultPlan(&(FFT<T>::planHandle), OpenCLBase::context(), FFT<T>::dim, clLengths);
    std::cout << __func__ << "(clfftCreateDefaultPlan) OpenCL Status: " << CLErrorToString( FFT<T>::err ) << std::endl;

    /* Set plan parameters. */
    if ( boost::is_same< typename T::value_type, float>::value ) {
        std::cout << __func__ << __CLASS__ << "Single Precision" << std::endl;
        FFT<T>::err = clfftSetPlanPrecision(FFT<T>::planHandle, CLFFT_SINGLE);
    } else {
        std::cout << __func__ << __CLASS__ << "Double Precision" << std::endl;
        FFT<T>::err = clfftSetPlanPrecision(FFT<T>::planHandle, CLFFT_DOUBLE);
    }

    std::cout << __func__ << "(clfftSetPlanPrecision) OpenCL Status: " << CLErrorToString(  FFT<T>::err ) << std::endl;
    FFT<T>::err = clfftSetLayout(FFT<T>::planHandle,  CLFFT_REAL,  CLFFT_HERMITIAN_INTERLEAVED );
    std::cout << __func__ << "(clfftSetLayout) OpenCL Status: " << CLErrorToString( FFT<T>::err ) << std::endl;
    FFT<T>::err = clfftSetResultLocation(FFT<T>::planHandle, CLFFT_INPLACE);
    std::cout << __func__ << "(clfftSetResultLocation) OpenCL Status: " << CLErrorToString( FFT<T>::err ) << std::endl;

    /* Bake the plan. */
    FFT<T>::err = clfftBakePlan(FFT<T>::planHandle, 1, &OpenCLBase::command_queue(), NULL, NULL);

}

template <typename T>
cl::Buffer& PowerSpectrum<T>::ProcessedSignal() {
    std::cout << __func__ << __CLASS__ << std::endl;
    return output_buff;
}

template <typename T>
size_t PowerSpectrum<T>::ProcessedSignalBytes() {
    std::cout << __func__ << __CLASS__ << std::endl;
    return TaskItem::signal_size*sizeof( typename T::value_type );
}

template <typename T>
size_t PowerSpectrum<T>::ProcessedSignalSize() {
    return TaskItem::signal_size;
}
