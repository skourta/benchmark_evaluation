#include <tiramisu/tiramisu.h>
#include <tiramisu/auto_scheduler/evaluator.h>
#include <tiramisu/auto_scheduler/search_method.h>
#include "function_heat2d_XLARGE_wrapper.h"


using namespace tiramisu;

int main(int argc, char **argv)
{
    tiramisu::init("function_heat2d_XLARGE");

    var i00("i00", 0, 2562 - 2) , i01("i01", 0, 3074 - 2) ;

    input input00("input00", {i00, i01}, p_int32);
    
    computation comp01("comp01", {i00, i01},  0);
    computation comp02("comp02", {i00, i01}, 3 * input00(i00 + 1, i01 + 1) +   4 * (input00(i00 + 1, i01 + 2) + input00(i00 + 1, i01) + input00(i00 + 2, i01 + 1) + input00(i00, i01 + 1)));
    
    comp01.then(comp02,i01);

    buffer buf02("buf02", {2562, 3074}, p_int32, a_output);
    buffer buf00("buf00", {2562, 3074}, p_int32, a_input);

//     comp02.store_in(&buf02);
//     input00.store_in(&buf00);
    comp01.store_in(&buf02);
    input00.store_in(&buf00);
    comp02.store_in(&buf02, {i00, i01});
    
    tiramisu::codegen({&buf02, &buf00}, "./function_heat2d_XLARGE.o");
    return 0;
}
