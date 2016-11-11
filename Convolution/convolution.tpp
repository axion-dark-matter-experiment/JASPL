template <typename T>
Convolution<T>::Convolution( T& convolution_kernel ) {

    static_assert( is_stdlib_container< T >::value, "Convolution can only accept container-like objects." );

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

    std::cout << __func__ << " OpenCL Status: " << CLErrorString( err ) << std::endl;
}

template <typename T>
void Convolution<T>::SetSignal( cl::Buffer& signal_buff, uint sig_size ){

    signal_size = sig_size;

    cl_int err;
    err = kernel.setArg(0, signal_buff);

    std::cout << __func__ << " OpenCL Status: " << CLErrorString( err ) << std::endl;

}
