__kernel void ScalarMultiply( __global TYPE* input, TYPE scalar )
{

    int i = get_global_id(0);

    input[i] *= scalar;

}
