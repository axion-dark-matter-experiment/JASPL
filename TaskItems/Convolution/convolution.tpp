template <typename T>
Convolution<T>::Convolution( T& convolution_kernel ) {

    static_assert( is_stdlib_container< T >::value, "Convolution can only accept pointers or container-like objects." );
    static_assert( std::is_arithmetic<typename T::value_type>::value, "Convolution must be made with arithmetic type" );

    std::string source_directory = SOURCE_DIR;
    std::string kernel_name = "/linearconvolve.cl";

    kernel_path = source_directory + kernel_name;
//    kernel_source = GetOpenCLSource( path );

    LoadCLKernel<typename T::value_type>( "LinearConvolve" );

    uint kernel_size = convolution_kernel.size();
    size_t kernel_bytes = kernel_size*sizeof( typename T::value_type );
    auto kernel_ptr = convolution_kernel.data();

    kernel_buff = cl::Buffer ( context, CL_MEM_READ_ONLY, kernel_bytes );
    command_queue.enqueueWriteBuffer( kernel_buff, CL_TRUE, 0, kernel_bytes, kernel_ptr );

    cl_int err;
    err = kernel.setArg(1, kernel_buff);
    std::cout << __func__ << " OpenCL Status: " << CLErrorToString( err ) << std::endl;
    err = kernel.setArg(4, kernel_size);
    std::cout << __func__ << " OpenCL Status: " << CLErrorToString( err ) << std::endl;
}

template <typename T>
Convolution<T>::Convolution( T* convolution_kernel ) {

    static_assert( std::is_arithmetic<T>::value, "Convolution must be made with arithmetic pointer type" );
    std::string source_directory = SOURCE_DIR;
    std::string kernel_name = "/linearconvolve.cl";

    kernel_path = source_directory + kernel_name;
//    kernel_source = GetOpenCLSource( path );

    LoadCLKernel<T>( "LinearConvolve" );

    uint kernel_size = convolution_kernel.size();
    size_t kernel_bytes = kernel_size*sizeof( typename T::value_type );
    auto kernel_ptr = convolution_kernel.data();

    kernel_buff = cl::Buffer ( context, CL_MEM_READ_ONLY, kernel_bytes );
    command_queue.enqueueWriteBuffer( kernel_buff, CL_TRUE, 0, kernel_bytes, kernel_ptr );

    cl_int err;
    err = kernel.setArg(1, kernel_buff);
    err = kernel.setArg(3, kernel_size);

    std::cout << __func__ << " OpenCL Status: " << CLErrorToString( err ) << std::endl;
}

template <typename T>
Convolution<T>::~Convolution() {}

template <typename T>
void Convolution<T>::Trigger() {

    cl_int err;
    err = command_queue.enqueueNDRangeKernel( kernel,cl::NullRange, cl::NDRange( signal_size ) );

    std::cout << __func__ << "(Convolution) OpenCL Status: " << CLErrorToString( err ) << std::endl;
}

template <typename T>
void Convolution<T>::SetSignal( cl::Buffer& signal_buff, uint sig_size ){

    signal_size = sig_size;

    cl_int* err_ptr = NULL;
    scratch_buff = cl::Buffer ( context, CL_MEM_READ_WRITE, sig_size*sizeof(typename T::value_type), err_ptr );
    std::cout << __func__ << " OpenCL Status: " << CLErrorToString( err_ptr ) << std::endl;

    cl_int err = kernel.setArg(2, scratch_buff);
    std::cout << __func__ << " OpenCL Status: " << CLErrorToString( err ) << std::endl;

    err = kernel.setArg(0, signal_buff);
    err = kernel.setArg(3, sig_size);

    std::cout << __func__ << " OpenCL Status: " << CLErrorToString( err ) << std::endl;
}

template <typename T>
cl::Buffer& Convolution<T>::ProcessedSignal() {
    std::cout << __func__ << __CLASS__ << std::endl;
    return scratch_buff;
}

template <typename T>
size_t Convolution<T>::ProcessedSignalBytes() {
    std::cout << __func__ << __CLASS__ << std::endl;
    return signal_size*sizeof( typename T::value_type );
}

template <typename T>
size_t Convolution<T>::ProcessedSignalSize(){
    return signal_size;
}
