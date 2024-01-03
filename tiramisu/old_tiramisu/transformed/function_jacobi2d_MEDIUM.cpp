#include <tiramisu/tiramisu.h>
#include <tiramisu/auto_scheduler/evaluator.h>
#include <tiramisu/auto_scheduler/search_method.h>
#include "function_jacobi2d_MEDIUM_wrapper.h"

using namespace tiramisu;
int main(int argc, char **argv)
{
    tiramisu::init("function_jacobi2d_MEDIUM");

    var i00("i00", 0, 250 - 2), i01("i01", 0, 100);

    input input00("input00", {i00, i01}, p_int32);

    computation comp02("comp02", {i00, i01}, 3 * input00(i00 + 1, i01 + 1) + 4 * (input00(i00 + 1, i01 + 2) + input00(i00 + 1, i01) + input00(i00 + 2, i01 + 1) + input00(i00, i01 + 1)));

    buffer buf02("buf02", {250, 100}, p_int32, a_output);
    buffer buf00("buf00", {250, 100}, p_int32, a_input);

    input00.store_in(&buf00);
    comp02.store_in(&buf02, {i00, i01});

    comp02.tag_parallel_level(0);

    comp02.tile(0, 1, 64, 64);

    tiramisu::codegen({&buf02, &buf00}, "function_jacobi2d_MEDIUM.o");

    return 0;
}
