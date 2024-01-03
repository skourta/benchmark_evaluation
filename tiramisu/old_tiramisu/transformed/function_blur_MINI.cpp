#include <tiramisu/tiramisu.h>
#include <tiramisu/auto_scheduler/evaluator.h>
#include <tiramisu/auto_scheduler/search_method.h>
#include "function_blur_MINI_wrapper.h"

using namespace tiramisu;

int main(int argc, char **argv)
{
    tiramisu::init("function_blur_MINI");

    // iterators
    var i00("i00", 0, 5), i01("i01", 0, 18 - 2), i02("i02", 0, 34 - 2);

    // computations
    input input00("input00", {i00, i01, i02}, p_int32);

    computation comp01("comp01", {i00, i01, i02}, input00(i00, i01, i02) + input00(i00, i01 + 1, i02) + input00(i00, i01 + 2, i02) + input00(i00, i01, i02 + 1) + input00(i00, i01 + 1, i02 + 1) + input00(i00, i01 + 2, i02 + 1) + input00(i00, i01, i02 + 2) + input00(i00, i01 + 1, i02 + 2) + input00(i00, i01 + 2, i02 + 2));

    // buffers
    buffer buf01("buf01", {5, 18, 34}, p_int32, a_output);
    buffer buf00("buf00", {5, 18, 34}, p_int32, a_input);

    comp01.store_in(&buf01);
    input00.store_in(&buf00);

    comp01.interchange(0, 1);

    comp01.unroll(2, 16);

    tiramisu::codegen({&buf01, &buf00}, "./function_blur_MINI.o");

    return 0;
}
