#include <tiramisu/tiramisu.h>
#include <tiramisu/auto_scheduler/evaluator.h>
#include <tiramisu/auto_scheduler/search_method.h>
#include "function_cvtcolor_XLARGE_wrapper.h"

using namespace tiramisu;

int main(int argc, char **argv)
{
    tiramisu::init("function_cvtcolor_XLARGE");
// iterators
    int c00(3074), c01(1282), c02(5);
    var i00("i00", 0, c00) , i01("i01", 0, c01) , i02("i02", 0, c02) ;
    
    // computations
    input input00("input00", {i00, i01, i02}, p_int32);
    input input01("input01", {i02}, p_int32);

    computation comp02("comp02", {i00, i01, i02}, p_int32);
    comp02.set_expression(comp02(i00, i01, i02) + input00(i00, i01, i02) * input01(i02));
    
    // buffers
    buffer buf02("buf02", {3074, 1282}, p_int32, a_output);
    buffer buf00("buf00", {3074, 1282, 5}, p_int32, a_input);
    buffer buf01("buf01", {5}, p_int32, a_input);

    comp02.store_in(&buf02, {i00, i01});
    input00.store_in(&buf00);
    input01.store_in(&buf01);
       
    tiramisu::codegen({&buf02, &buf00, &buf01}, "./function_cvtcolor_XLARGE.o");
    return 0;
}
