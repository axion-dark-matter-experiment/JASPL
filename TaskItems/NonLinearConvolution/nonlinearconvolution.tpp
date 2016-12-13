template <typename T>
NonLinearConvolution<T>::NonLinearConvolution( T& convolution_kernel ) {

    static_assert( is_stdlib_container< T >::value, "NonLinearConvolution can only accept pointers or container-like objects." );
    static_assert( std::is_arithmetic<typename T::value_type>::value, "NonLinearConvolution must be made with arithmetic type" );

    std::string source_directory = SOURCE_DIR;
    std::string kernel_name = "/nonlinearconvolve.cl";

    kernel_path = source_directory + kernel_name;

    LoadCLKernel<typename T::value_type>( "NonLinearConvolve" );

    uint kernel_size = convolution_kernel.size();
    size_t kernel_bytes = kernel_size*sizeof( typename T::value_type );
    auto kernel_ptr = convolution_kernel.data();

    kernel_buff = cl::Buffer ( context, CL_MEM_READ_ONLY, kernel_bytes );
    command_queue.enqueueWriteBuffer( kernel_buff, CL_TRUE, 0, kernel_bytes, kernel_ptr );

    cl_int err;
    err = kernel.setArg(1, kernel_buff);
    OCL_DEBUG( err );
    err = kernel.setArg(4, kernel_size);
    OCL_DEBUG( err );
}

template <typename T>
NonLinearConvolution<T>::NonLinearConvolution( T* convolution_kernel ) {

    static_assert( std::is_arithmetic<T>::value, "NonLinearConvolution must be made with arithmetic pointer type" );
    std::string source_directory = SOURCE_DIR;
    std::string kernel_name = "/linearconvolve.cl";

    kernel_path = source_directory + kernel_name;

    LoadCLKernel<T>( "LinearConvolve" );

    uint kernel_size = convolution_kernel.size();
    size_t kernel_bytes = kernel_size*sizeof( typename T::value_type );
    auto kernel_ptr = convolution_kernel.data();

    kernel_buff = cl::Buffer ( context, CL_MEM_READ_ONLY, kernel_bytes );
    command_queue.enqueueWriteBuffer( kernel_buff, CL_TRUE, 0, kernel_bytes, kernel_ptr );

    cl_int err;
    err = kernel.setArg(1, kernel_buff);
    OCL_DEBUG( err );
    err = kernel.setArg(3, kernel_size);
    OCL_DEBUG( err );
}

template <typename T>
NonLinearConvolution<T>::~NonLinearConvolution() {}

template <typename T>
void NonLinearConvolution<T>::Trigger() {

    cl_int err;
    err = command_queue.enqueueNDRangeKernel( kernel,cl::NullRange, cl::NDRange( signal_size ) );
    OCL_DEBUG( err );
}

template <typename T>
void NonLinearConvolution<T>::SetSignal( cl::Buffer& signal_buff, uint sig_size ){

    signal_size = sig_size;

    cl_int* err_ptr = NULL;
    scratch_buff = cl::Buffer ( context, CL_MEM_READ_WRITE, sig_size*sizeof(typename T::value_type), err_ptr );
    OCL_DEBUG( err_ptr );

    cl_int err = kernel.setArg(2, scratch_buff);
    OCL_DEBUG( err );

    err = kernel.setArg(0, signal_buff);
    err = kernel.setArg(3, sig_size);
    OCL_DEBUG( err );
}

template <typename T>
cl::Buffer& NonLinearConvolution<T>::ProcessedSignal() {
    return scratch_buff;
}

template <typename T>
size_t NonLinearConvolution<T>::ProcessedSignalBytes() {
    return signal_size*sizeof( typename T::value_type );
}

template <typename T>
size_t NonLinearConvolution<T>::ProcessedSignalSize(){
    return signal_size;
}
