#include <tiramisu/tiramisu.h>
#include <tiramisu/auto_scheduler/evaluator.h>
#include <tiramisu/auto_scheduler/search_method.h>
#include "function_heat3d_LARGE_wrapper.h"

using namespace tiramisu;

int main(int argc, char **argv)
{
    tiramisu::init("function_heat3d_LARGE");

    var i00("i00", 0, 500 - 2), i01("i01", 0, 120 - 2), i02("i02", 0, 120 - 2);

    input input00("input00", {i00, i01, i02}, p_int32);

    computation comp02("comp02", {i00, i01, i02}, input00(i00, i01 + 1, i02 + 1) + 6 * input00(i00 + 1, i01 + 1, i02 + 1) - input00(i00 + 2, i01 + 1, i02 + 1) + input00(i00 + 1, i01, i02 + 1) - input00(i00 + 1, i01 + 2, i02 + 1) + input00(i00 + 1, i01 + 1, i02) - input00(i00 + 1, i01 + 1, i02 + 2));

    buffer buf02("buf02", {500, 120, 120}, p_int32, a_output);
    buffer buf00("buf00", {500, 120, 120}, p_int32, a_input);

    comp02.store_in(&buf02);
    input00.store_in(&buf00);

    comp02.interchange(0, 1);

    comp02.tag_parallel_level(0);

    comp02.tile(1, 2, 32, 64);

    tiramisu::codegen({&buf02, &buf00}, "./function_heat3d_LARGE.o");

    return 0;
}
