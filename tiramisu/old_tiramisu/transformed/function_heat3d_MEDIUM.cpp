#include <tiramisu/tiramisu.h>
#include <tiramisu/auto_scheduler/evaluator.h>
#include <tiramisu/auto_scheduler/search_method.h>
#include "function_heat3d_MEDIUM_wrapper.h"

using namespace tiramisu;

int main(int argc, char **argv)
{
    tiramisu::init("function_heat3d_MEDIUM");

    var i00("i00", 0, 100 - 2), i01("i01", 0, 40 - 2), i02("i02", 0, 40 - 2);

    input input00("input00", {i00, i01, i02}, p_int32);

    computation comp02("comp02", {i00, i01, i02}, input00(i00, i01 + 1, i02 + 1) + 6 * input00(i00 + 1, i01 + 1, i02 + 1) - input00(i00 + 2, i01 + 1, i02 + 1) + input00(i00 + 1, i01, i02 + 1) - input00(i00 + 1, i01 + 2, i02 + 1) + input00(i00 + 1, i01 + 1, i02) - input00(i00 + 1, i01 + 1, i02 + 2));

    buffer buf02("buf02", {100, 40, 40}, p_int32, a_output);
    buffer buf00("buf00", {100, 40, 40}, p_int32, a_input);

    comp02.store_in(&buf02);
    input00.store_in(&buf00);

    comp02.interchange(1, 2);

    comp02.tag_parallel_level(0);

    tiramisu::codegen({&buf02, &buf00}, "./function_heat3d_MEDIUM.o");

    return 0;
}
