#include <tiramisu/tiramisu.h>
#include <tiramisu/auto_scheduler/evaluator.h>
#include <tiramisu/auto_scheduler/search_method.h>
#include "function_convolution_MEDIUM_wrapper.h"
using namespace tiramisu;

int main(int argc, char **argv)
{
    tiramisu::init("function_convolution_MEDIUM");

    var n("n", 0, 8), fou("fou", 0, 2), y("y", 0, 1024), x("x", 0, 1024), fin("fin", 0, 3), k_y("k_y", 0, 3), k_x("k_x", 0, 3), y_pad("y_pad", 0, 1026), x_pad("x_pad", 0, 1026);

    // computations
    input bias("bias", {fou}, p_int32);
    input src("src", {n, fin, y_pad, x_pad}, p_int32);
    input weights("weights", {n, fou, y, x}, p_int32);

    computation conv_init("conv_init", {n, fou, y, x}, bias(fou));
    computation conv("conv", {n, fou, y, x, fin, k_y, k_x}, p_int32);
    conv.set_expression(conv(n, fou, y, x, fin, k_y, k_x) + src(n, fin, y + k_y, x + k_x) * weights(fou, fin, k_y, k_x));

    conv_init.then(conv, x);

    // buffers
    buffer buf_bias("buf_bias", {2}, p_int32, a_input);
    buffer buf_src("buf_src", {8, 3, 1026, 1026}, p_int32, a_input);
    buffer buf_weights("buf_weights", {2, 3, 3, 3}, p_int32, a_input);

    buffer buf_output("buf_output", {8, 2, 1024, 1024}, p_int32, a_output);

    bias.store_in(&buf_bias);
    src.store_in(&buf_src);
    weights.store_in(&buf_weights);

    conv_init.store_in(&buf_output);
    conv.store_in(&buf_output, {n, fou, y, x});

    conv_init.interchange(0, 1);
    conv.interchange(0, 1);

    conv_init.tag_parallel_level(2);

    tiramisu::codegen({&buf_output, &buf_bias, &buf_src, &buf_weights}, "./function_convolution_MEDIUM.o");

    return 0;
}
