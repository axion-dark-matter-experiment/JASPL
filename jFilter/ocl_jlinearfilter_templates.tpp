//template <typename T>
//std::string FakeKernelTemplating( T type, std::string kernel_source ) {;

//    std::string type_str = get_type_name( type );
//    boost::replace_all(kernel_source, "TYPE", type_str );

//    return kernel_source;
//}

//template <typename T>
//void JLinearConvolve::LoadCLKernel(T type) {

//    sources.clear();
//    std::string kernel_source = GetOpenCLKernel();

//    std::string local_kernel = FakeKernelTemplating( type, kernel_source );

//    sources.push_back({local_kernel.c_str(),local_kernel.length()});

//    program = cl::Program (context,sources);

//    if( program.build({current_device}) != CL_SUCCESS ) {

//        std::string err_str = __FUNCTION__;
//        err_str += "Error Building OpenCL Program\n ";
//        err_str += "Compilier Output:\n ";
//        err_str += program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(current_device);
//        err_str += "\n";
//        throw std::runtime_error( err_str );
//    }

//    kernel = cl::Kernel(program, "TemplatedLinearConvolve" );

//}

template <class T>
JVector<T> JLinearConvolve::Convolve(JVector<T>& signal, JVector<T>& kernel_vec) {

    T type;
    LoadCLKernel( type, "TemplatedLinearConvolve" );
    check_if_arithmetic( type );

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
