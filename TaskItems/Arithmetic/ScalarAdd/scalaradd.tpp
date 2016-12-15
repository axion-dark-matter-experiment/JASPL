template <typename T>
ScalarAdd<T>::ScalarAdd( typename T::value_type scalar_value ) {

    static_assert( is_stdlib_container< T >::value, "ScalarAdd can only accept pointers or container-like objects." );
    static_assert( std::is_arithmetic< typename T::value_type >::value, "ScalarAdd must be made with arithmetic type" );

    std::string source_directory = SOURCE_DIR;
    std::string kernel_name = "/scalaradd.cl";

    kernel_path = source_directory + kernel_name;

    LoadCLKernel<typename T::value_type>( "ScalarAdd" );

    cl_int err;
    err = kernel.setArg( 1, scalar_value );
    OCL_DEBUG( err );
}

template <typename T>
ScalarAdd<T>::~ScalarAdd() {}

template <typename T>
void ScalarAdd<T>::Trigger() {

    cl_int err;
    err = command_queue.enqueueNDRangeKernel( kernel,cl::NullRange, cl::NDRange( signal_size ) );
    OCL_DEBUG( err );
}

template <typename T>
void ScalarAdd<T>::SetSignal( cl::Buffer& signal_buff, uint sig_size ){

    signal_size = sig_size;
    err = kernel.setArg( 0, signal_buff );

    OCL_DEBUG( err );
}

template <typename T>
cl::Buffer& ScalarAdd<T>::ProcessedSignal() {
    //
}

template <typename T>
size_t ScalarAdd<T>::ProcessedSignalBytes() {
    return signal_size*sizeof( typename T::value_type );
}

template <typename T>
size_t ScalarAdd<T>::ProcessedSignalSize(){
    return signal_size;
}

template <typename T>
bool ScalarAdd<T>::NeedsToReknew() {
    return false;
}
