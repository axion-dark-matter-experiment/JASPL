template <class T>
JVector<T> JLinearConvolve::Convolve(JVector<T>& signal, JVector<T>& kernel_vec) {

    check_if_arithmetic( signal.at(0) );
    check_if_arithmetic( kernel_vec.at(0) );

    uint signal_size = signal.size();
    size_t signal_bytes = signal_size*sizeof(T);
    T* signal_ptr = signal.data();

    uint kernel_size = kernel_vec.size();
    size_t kernel_bytes = kernel_size*sizeof(T);
    T* kernel_ptr = kernel_vec.data();


    //Create memory buffers
    inputCL = clCreateBuffer(context, CL_MEM_READ_ONLY, signal_bytes, NULL, NULL);
    kernelCL = clCreateBuffer(context, CL_MEM_READ_ONLY, kernel_bytes, NULL, NULL);
    outputCL = clCreateBuffer(context, CL_MEM_WRITE_ONLY, signal_bytes, NULL, NULL);

    // Write our data set into the input array in device memory
    ret = clEnqueueWriteBuffer(command_queue, inputCL, CL_TRUE, 0, signal_bytes, signal_ptr, 0, NULL, NULL);
    ret |= clEnqueueWriteBuffer(command_queue, kernelCL, CL_TRUE, 0, kernel_bytes, kernel_ptr, 0, NULL, NULL);

    /* Set OpenCL Kernel Parameters */

    /* input buffer, arg 0 */
    clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&inputCL);
    /* filter buffer, arg 1 */
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&kernelCL);
    /* output buffer, arg 2 */
    clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&outputCL);
    /* input image width, arg 3*/
    clSetKernelArg(kernel, 3, sizeof(int), (void *)&signal_size);
    /* filter width, arg 4*/
    clSetKernelArg(kernel, 4, sizeof(int), (void *)&kernel_size);

    // Number of work items in each local work group
    local_size = 64;

    // Number of total work items - localSize must be devisor
    global_size = ceil( signal_size/(float)local_size)*local_size;

    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);

    // Wait for the command queue to get serviced before reading back results
    clFinish(command_queue);

    T* processed_signal = (T*)malloc(signal_bytes);

    // Read the results from the device
    clEnqueueReadBuffer(command_queue, outputCL, CL_TRUE, 0, signal_bytes, processed_signal, 0, NULL, NULL );

    ret = clReleaseMemObject(inputCL);
    ret = clReleaseMemObject(kernelCL);
    ret = clReleaseMemObject(outputCL);

    JVector<T> processed_vector( processed_signal, signal_size );

    free (processed_signal);

    return processed_vector;
}
