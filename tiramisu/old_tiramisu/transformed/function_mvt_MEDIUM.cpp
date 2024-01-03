#include <tiramisu/tiramisu.h>
#include <tiramisu/auto_scheduler/evaluator.h>
#include <tiramisu/auto_scheduler/search_method.h>
#include "function_mvt_MEDIUM_wrapper.h"

using namespace tiramisu;
int main(int argc, char **argv)
{
    tiramisu::init("function_mvt_MEDIUM");
    // iterators
    var i00("i00", 0, 400), i01("i01", 0, 400);

    // computations
    input input00("input00", {i00, i01}, p_int32);
    input input01("input01", {i00}, p_int32);
    input input02("input02", {i00}, p_int32);

    computation comp03("comp03", {i00, i01}, p_int32);
    computation comp04("comp04", {i00, i01}, p_int32);
    comp03.set_expression(comp03(i00, i01) + input00(i00, i01) * input01(i01));
    comp04.set_expression(comp04(i00, i01) + input00(i01, i00) * input02(i01));

    comp03.then(comp04, i01);

    // buffers
    buffer buf03("buf03", {400}, p_int32, a_output);
    buffer buf04("buf04", {400}, p_int32, a_output);
    buffer buf00("buf00", {400, 400}, p_int32, a_input);
    buffer buf01("buf01", {400}, p_int32, a_input);
    buffer buf02("buf02", {400}, p_int32, a_input);

    comp03.store_in(&buf03, {i00});
    comp04.store_in(&buf04, {i00});
    input00.store_in(&buf00);
    input01.store_in(&buf01);
    input02.store_in(&buf02);

    comp03.tag_parallel_level(0);

    comp03.tile(0, 1, 32, 32);
    comp04.tile(0, 1, 32, 32);
    clear_implicit_function_sched_graph();
    comp03.then(comp04, 3);

    tiramisu::codegen({&buf03, &buf04, &buf00, &buf01, &buf02}, "./function_mvt_MEDIUM.o");

    return 0;
}
