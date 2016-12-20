
template<typename T>
PowerSpectrum<T>::PowerSpectrum() {


    std::string kernel_str = R"END(
         __kernel void PowerSpectrum( __global TYPE* input, __global TYPE* scratch, const unsigned int signal_size )
         {

             int i = get_global_id(0);
             //Do not use sqrt(), you -will- get round-off errors for large FFT sizes
             scratch[i] = input[2*i]*input[2*i] + input[2*i+1]*input[2*i+1];
             //Normalize by number of elements
             scratch[i] /= (TYPE)signal_size * (TYPE)signal_size;

         }
    )END";

    std::string kernel_name = "PowerSpectrum";

//    std::string source_directory = SOURCE_DIR;
//    std::string kernel_name = "/powerspectrum.cl";

//    FFT<T>::kernel_path = source_directory + kernel_name;

//    TaskItem::LoadCLKernel< typename T::value_type >( "PowerSpectrum" );

    TaskItem::LoadCLKernel<typename T::value_type>( kernel_str, kernel_name );
}

template<typename T>
void PowerSpectrum<T>::Trigger() {

    FFT<T>::Trigger();
    //Since this is a two-step process we need to explictly call CommandQueue::finish() between steps
    TaskItem::command_queue.finish();

    FFT<T>::err = TaskItem::command_queue.enqueueNDRangeKernel( TaskItem::kernel,cl::NullRange, cl::NDRange( TaskItem::signal_size ) );
    TaskItem::signal_size = (TaskItem::signal_size%2 == 0)?( TaskItem::signal_size/2 ):( (TaskItem::signal_size - 1)/2 );
    OCL_DEBUG( FFT<T>::err );

}

template <typename T>
void PowerSpectrum<T>::SetSignal( cl::Buffer& signal_buff, uint sig_size ) {

    FFT<T>::err = TaskItem::kernel.setArg(0, signal_buff);
    OCL_DEBUG( FFT<T>::err );
    //std::cout << __func__ << "(setKernelArgs[0]) OpenCL Status: " << CLErrorToString( FFT<T>::err ) << std::endl;

    TaskItem::signal_size = sig_size;
    FFT<T>::local_buff = signal_buff;

    size_t scratch_bytes = (sig_size%2 == 0)?( sig_size*sizeof(typename T::value_type)/2 ):( (sig_size*sizeof(typename T::value_type) - 1)/2 );
    output_buff = cl::Buffer ( OpenCLBase::context, CL_MEM_READ_WRITE, scratch_bytes );

    FFT<T>::err = TaskItem::kernel.setArg(1, output_buff);
    OCL_DEBUG( FFT<T>::err );

    FFT<T>::err = TaskItem::kernel.setArg(2, sig_size);
    OCL_DEBUG( FFT<T>::err );

    size_t clLengths[1] = { sig_size };

    /* Create a default plan for a complex FFT. */
    FFT<T>::err = clfftCreateDefaultPlan(&(FFT<T>::planHandle), OpenCLBase::context(), FFT<T>::dim, clLengths);
    OCL_DEBUG( FFT<T>::err );

    /* Set plan parameters. */
    if ( boost::is_same< typename T::value_type, float>::value ) {
        FFT<T>::err = clfftSetPlanPrecision(FFT<T>::planHandle, CLFFT_SINGLE);
    } else {
        FFT<T>::err = clfftSetPlanPrecision(FFT<T>::planHandle, CLFFT_DOUBLE);
    }

    OCL_DEBUG( FFT<T>::err );

    FFT<T>::err = clfftSetLayout(FFT<T>::planHandle,  CLFFT_REAL,  CLFFT_HERMITIAN_INTERLEAVED );
    OCL_DEBUG( FFT<T>::err );

    FFT<T>::err = clfftSetResultLocation(FFT<T>::planHandle, CLFFT_INPLACE);
    OCL_DEBUG( FFT<T>::err );

    /* Bake the plan. */
    FFT<T>::err = clfftBakePlan(FFT<T>::planHandle, 1, &OpenCLBase::command_queue(), NULL, NULL);

}

template <typename T>
cl::Buffer& PowerSpectrum<T>::ProcessedSignal() {
    return output_buff;
}

template <typename T>
size_t PowerSpectrum<T>::ProcessedSignalBytes() {
    return TaskItem::signal_size*sizeof( typename T::value_type );
}

template <typename T>
size_t PowerSpectrum<T>::ProcessedSignalSize() {
    return TaskItem::signal_size;
}
