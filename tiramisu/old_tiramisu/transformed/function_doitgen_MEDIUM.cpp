#include <tiramisu/tiramisu.h>
#include <tiramisu/auto_scheduler/evaluator.h>
#include <tiramisu/auto_scheduler/search_method.h>
#include "function_doitgen_MEDIUM_wrapper.h"

using namespace tiramisu;

int main(int argc, char **argv)
{
    tiramisu::init("function_doitgen_MEDIUM");
    //     constant c00("c00", 3), c01("c01", 768), c02("c02", 384);

    var i00("i00", 0, 40), i01("i01", 0, 60), i02("i02", 0, 50), i03("i03", 0, 50);

    input input00("input00", {i00, i01, i03}, p_int32);
    input input01("input01", {i02, i03}, p_int32);

    computation comp03("comp03", {i00, i01, i02, i03}, p_int32);
    comp03.set_expression(comp03(i00, i01, i02, i03) + input00(i00, i01, i03) * input01(i02, i03));

    buffer buf03("buf03", {40, 60, 50}, p_int32, a_output);
    buffer buf00("buf00", {40, 60, 50}, p_int32, a_input);
    buffer buf01("buf01", {50, 50}, p_int32, a_input);

    comp03.store_in(&buf03, {i00, i01, i02});
    input00.store_in(&buf00);
    input01.store_in(&buf01);
    comp03.interchange(2, 3);

    comp03.tag_parallel_level(0);

    comp03.tile(2, 3, 32, 32);

    tiramisu::codegen({&buf03, &buf00, &buf01}, "./function_doitgen_MEDIUM.o");

    return 0;
}
