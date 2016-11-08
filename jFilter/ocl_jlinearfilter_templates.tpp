template <class T>
JVector<T> JLinearConvolve::Convolve(JVector<T>& signal, JVector<T>& kernel_vec) {

    LoadCLKernel<T>( "TemplatedLinearConvolve" );
    check_if_arithmetic< T >();

    uint signal_size = signal.size();
    size_t signal_bytes = signal_size*sizeof(T);
    T* signal_ptr = signal.data();

    uint kernel_size = kernel_vec.size();
    size_t kernel_bytes = kernel_size*sizeof(T);
    T* kernel_ptr = kernel_vec.data();

    inputCL = cl::Buffer ( context, CL_MEM_READ_ONLY, signal_bytes );
    kernelCL = cl::Buffer ( context, CL_MEM_READ_ONLY, kernel_bytes );
    outputCL = cl::Buffer ( context, CL_MEM_WRITE_ONLY, signal_bytes );

    command_queue.enqueueWriteBuffer( inputCL, CL_TRUE, 0, signal_bytes, signal_ptr );
    command_queue.enqueueWriteBuffer( kernelCL, CL_TRUE, 0, kernel_bytes, kernel_ptr );

    kernel.setArg(0, inputCL);
    kernel.setArg(1, kernelCL);
    kernel.setArg(2, outputCL);
    kernel.setArg(3, signal_size);
    kernel.setArg(4, kernel_size);

    command_queue.enqueueNDRangeKernel( kernel,cl::NullRange, cl::NDRange( signal_size ) );
    command_queue.finish();

    T* processed_signal = (T*)malloc(signal_bytes);

    command_queue.enqueueReadBuffer(outputCL, CL_TRUE,0, signal_bytes, processed_signal);

    JVector<T> processed_vector( processed_signal, signal_size );

    free (processed_signal);

    return processed_vector;
}
