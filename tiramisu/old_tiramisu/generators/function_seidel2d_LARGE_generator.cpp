#include <tiramisu/tiramisu.h>
#include <tiramisu/auto_scheduler/evaluator.h>
#include <tiramisu/auto_scheduler/search_method.h>
#include "function_seidel2d_LARGE_wrapper.h"

using namespace tiramisu;

int main(int argc, char **argv)
{
    tiramisu::init("function_seidel2d_LARGE");
    int c00(2000);
    var i00("i00", 0, c00 - 2) , i01("i01", 0, c00 - 2) ;
    
    // computations
    input input00("input00", {i00, i01}, p_int32);

    computation comp01("comp01", {i00, i01}, p_int32);
    comp01.set_expression(9 * (input00(i00, i01) + input00(i00, i01 + 1) + input00(i00, i01 + 2) + input00(i00 + 1, i01) + input00(i00 + 1, i01 + 1) + input00(i00 + 1, i01 + 2) + input00(i00 + 2, i01) + input00(i00 + 2, i01 + 1) + input00(i00 + 2, i01 + 2)));
    
    // buffers
    buffer buf01("buf01", {2000, 2000}, p_int32, a_output);
    buffer buf00("buf00", {2000, 2000}, p_int32, a_input);

    comp01.store_in(&buf01);
    input00.store_in(&buf00);
    
    tiramisu::codegen({&buf01, &buf00}, "function_seidel2d_LARGE.o");
    
    return 0;
}
