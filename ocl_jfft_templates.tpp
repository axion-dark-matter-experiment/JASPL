template <typename T> void JFFT::PowerSpectrum( T &input ) {

    uint N = input.size();
    size_t clLengths[1] = {N};

    float *X;
    X = (float *)malloc(N * 2 * sizeof(*X));

    for ( uint i = 0; i < N ; i++ ) {

        X[2*i] = static_cast<float> (input[i]); //Real Part
        X[2*i+1] = 0.0;//Imaginary Part

    }

    cl_int err;
    cl_mem bufX;
    /* Prepare OpenCL memory objects and place data inside them. */
    bufX = clCreateBuffer( ctx, CL_MEM_READ_WRITE, N * 2 * sizeof(*X), NULL, &err );

    err = clEnqueueWriteBuffer( queue, bufX, CL_TRUE, 0, N * 2 * sizeof( *X ), X, 0, NULL, NULL );

    /* Create a default plan for a complex FFT. */
    err = clfftCreateDefaultPlan(&planHandle, ctx, dim, clLengths);

    /* Set plan parameters. */
    err = clfftSetPlanPrecision(planHandle, CLFFT_SINGLE);
    err = clfftSetLayout(planHandle, CLFFT_COMPLEX_INTERLEAVED, CLFFT_COMPLEX_INTERLEAVED);
    err = clfftSetResultLocation(planHandle, CLFFT_INPLACE);

    /* Bake the plan. */
    err = clfftBakePlan(planHandle, 1, &queue, NULL, NULL);

    /* Execute the plan. */
    err = clfftEnqueueTransform(planHandle, CLFFT_FORWARD, 1, &queue, 0, NULL, NULL, &bufX, NULL, NULL);

    /* Wait for calculations to be finished. */
    err = clFinish(queue);

    /* Fetch results of calculations. */
    err = clEnqueueReadBuffer( queue, bufX, CL_TRUE, 0, N * 2 * sizeof( *X ), X, 0, NULL, NULL );

    /* Release OpenCL memory objects. */
    clReleaseMemObject( bufX );

    for (uint i = 0; i < N ; i++ ) {
        input[i] = pow(X[2*i], 2.0) + pow(X[2*i+1],2.0);
    }

    free(X);
}
