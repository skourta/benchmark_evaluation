#include <tiramisu/tiramisu.h>
#include <tiramisu/auto_scheduler/evaluator.h>
#include <tiramisu/auto_scheduler/search_method.h>
#include "function_matmul_SMALL_wrapper.h"

using namespace tiramisu;

int main(int argc, char **argv)
{
    tiramisu::init("function_matmul_SMALL");

    // -------------------------------------------------------
    // Layer I
    // -------------------------------------------------------
    var i00("i00", 0, 64) , i01("i01", 0, 96) , i02("i02", 0, 128) ;
    //inputs
    input input00("input00", {i00, i02}, p_int32);
    input input01("input01", {i02, i01}, p_int32);
    
    //Computations
    computation comp02("comp02", {i00, i01, i02}, p_int32);
    comp02.set_expression(comp02(i00, i01, i02) + input00(i00, i02) * input01(i02, i01));
    // -------------------------------------------------------
    // Layer III
    // -------------------------------------------------------
    //Input Buffers
    buffer buf02("buf02", {64, 96}, p_int32, a_output);
    buffer buf00("buf00", {64, 128}, p_int32, a_input);
    buffer buf01("buf01", {128, 96}, p_int32, a_input);

    comp02.store_in(&buf02, {i00, i01});
    input00.store_in(&buf00);
    input01.store_in(&buf01);
    
    // -------------------------------------------------------
    // Code Generation
    // -------------------------------------------------------
    tiramisu::codegen({&buf02, &buf00, &buf01}, "function_matmul_SMALL.o");
    
    return 0;
}
