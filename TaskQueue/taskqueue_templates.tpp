template <typename T>
TaskQueue<T>::TaskQueue() {
    std::cout << "Built using something else" << std::endl;
    static_assert( is_stdlib_container< T >::value, "TaskQueue can only accept pointer or container-like objects." );
}

template <typename T>
TaskQueue<T>::~TaskQueue() {}

template <typename T>
void TaskQueue<T>::Load( T signal ) {

    signal_size = signal.size();
    signal_bytes = signal_size*sizeof( typename T::value_type );

    auto signal_ptr = signal.data();

    cl_int* err = NULL;
    signal_input = cl::Buffer ( context, CL_MEM_READ_WRITE, signal_bytes, err );
    scratch_space = cl::Buffer ( context, CL_MEM_READ_WRITE, signal_bytes );
    output = cl::Buffer ( context, CL_MEM_WRITE_ONLY, signal_bytes );

    std::cout << __func__ << " OpenCL Status: " << CLErrorString( err ) << std::endl;

    //Load signal into global memory
    cl_int err_2;
    err_2 = command_queue.enqueueWriteBuffer( signal_input, CL_TRUE, 0, signal_bytes, signal_ptr );
    std::cout << __func__ << " OpenCL Status 2: " << CLErrorString( err_2 ) << std::endl;

//    command_queue.finish();

}

template <typename T>
T TaskQueue<T>::Recall() {

    auto processed_signal = ( typename T::value_type* )malloc(signal_bytes);

    command_queue.enqueueReadBuffer (signal_input, CL_TRUE, 0, signal_bytes, processed_signal);
//    command_queue.finish();

    T processed_vector( processed_signal, processed_signal + signal_size );

    free (processed_signal);

    return processed_vector;
}


template <typename T>
TaskQueue<T*>::TaskQueue() {
    std::cout << "Built using pointer" << std::endl;
    static_assert( std::is_arithmetic< T >::value, "TaskQueue can only accept arithemic types." );
}

template <typename T>
void TaskQueue<T*>::Load( T* signal_ptr ) {

    signal_size = sizeof(signal_ptr)/sizeof(signal_ptr[0]);
    signal_bytes = sizeof(signal_ptr);

    signal_input = cl::Buffer ( context,  CL_MEM_READ_WRITE, signal_bytes );
    output = cl::Buffer ( context, CL_MEM_WRITE_ONLY, signal_bytes );

    //Load signal into global memory
    command_queue.enqueueWriteBuffer( signal_input, CL_TRUE, 0, signal_bytes, signal_ptr );
    command_queue.finish();
}


template< typename T>
T* TaskQueue<T*>::Recall() {
    T* processed_signal = (T*)malloc(signal_bytes);

    command_queue.enqueueReadBuffer (output, CL_TRUE, 0, signal_bytes, processed_signal);

    return processed_signal;
}
