template <typename T>
Convolution<T>::Convolution( T& convolution_kernel ) {

    static_assert( is_stdlib_container< T >::value, "Convolution can only accept container-like objects." );

    EstablishKernelPath( "/home/bephillips2/Qt-Projects/JASPL/Convolution/linearconvolve.cl" );
    kernel_source = GetOpenCLSource( "/home/bephillips2/Qt-Projects/JASPL/Convolution/linearconvolve.cl" );

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
