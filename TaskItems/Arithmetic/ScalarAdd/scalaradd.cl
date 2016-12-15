__kernel void ScalarAdd( __global TYPE* input, const TYPE scalar )
{

    int i = get_global_id(0);

    input[i] += scalar;
}
