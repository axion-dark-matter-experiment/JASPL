template <typename T>
std::string TaskItem::FakeKernelTemplating( std::string kernel_source ) {

    std::string type_str = get_type_name<T>();
    boost::replace_all( kernel_source, "TYPE", type_str );

    return kernel_source;
}

template <typename T>
void TaskItem::LoadCLKernel( std::string kernel_name ) {

    sources.clear();

    std::string kernel_source = GetOpenCLSource( kernel_path );
    std::string local_kernel = FakeKernelTemplating<T>( kernel_source );

    sources.push_back({local_kernel.c_str(),local_kernel.length()});

    program = cl::Program (context,sources);

    if( program.build({current_device}) != CL_SUCCESS ) {

        std::string err_str = __FUNCTION__;
        err_str += "\nError Building OpenCL Program\n ";
        err_str += "Compilier Output:\n ";
        err_str += program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(current_device);
        err_str += "\n";
        throw std::runtime_error( err_str );
    }

    cl_int err = 0;
    kernel = cl::Kernel(program, kernel_name.c_str() );
    OCL_DEBUG( err );

}
