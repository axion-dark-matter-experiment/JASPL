template <typename T>
ScalarMultiply<T>::ScalarMultiply( typename T::value_type scalar_value ) {

    static_assert( is_stdlib_container< T >::value, "ScalarMultiply can only accept pointers or container-like objects." );
    static_assert( std::is_arithmetic< typename T::value_type >::value, "ScalarMultiply must be made with arithmetic type" );

    std::string source_directory = SOURCE_DIR;
    std::string kernel_name = "/scalarmultiply.cl";

    kernel_path = source_directory + kernel_name;

    LoadCLKernel<typename T::value_type>( "ScalarMultiply" );

    cl_int err;
    err = kernel.setArg(1, scalar_value);
    OCL_DEBUG( err );
}

template <typename T>
ScalarMultiply<T>::~ScalarMultiply() {}

template <typename T>
void ScalarMultiply<T>::Trigger() {

    cl_int err;
    err = command_queue.enqueueNDRangeKernel( kernel,cl::NullRange, cl::NDRange( signal_size ) );
    OCL_DEBUG( err );
}

template <typename T>
void ScalarMultiply<T>::SetSignal( cl::Buffer& signal_buff, uint sig_size ){

    signal_size = sig_size;
    err = kernel.setArg(0, signal_buff);

    OCL_DEBUG( err );
}

template <typename T>
cl::Buffer& ScalarMultiply<T>::ProcessedSignal() {
    //
}

template <typename T>
size_t ScalarMultiply<T>::ProcessedSignalBytes() {
    return signal_size*sizeof( typename T::value_type );
}

template <typename T>
size_t ScalarMultiply<T>::ProcessedSignalSize(){
    return signal_size;
}

template <typename T>
bool ScalarMultiply<T>::NeedsToReknew() {
    return false;
}
