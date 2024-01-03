#include <tiramisu/tiramisu.h>
#include <tiramisu/auto_scheduler/evaluator.h>
#include <tiramisu/auto_scheduler/search_method.h>
#include "function_mvt_SMALL_wrapper.h"

using namespace tiramisu;
int main(int argc, char **argv)
{
    tiramisu::init("function_mvt_SMALL");
    // iterators
    var i00("i00", 0, 120) , i01("i01", 0, 120);
    
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
    buffer buf03("buf03", {120}, p_int32, a_output);
    buffer buf04("buf04", {120}, p_int32, a_output);
    buffer buf00("buf00", {120, 120}, p_int32, a_input);
    buffer buf01("buf01", {120}, p_int32, a_input);
    buffer buf02("buf02", {120}, p_int32, a_input);

    comp03.store_in(&buf03, {i00});
    comp04.store_in(&buf04, {i00});
    input00.store_in(&buf00);
    input01.store_in(&buf01);
    input02.store_in(&buf02);

    tiramisu::codegen({&buf03, &buf04, &buf00, &buf01, &buf02}, "./function_mvt_SMALL.o");
    
    return 0;
}
