#include <tiramisu/tiramisu.h>
#include <tiramisu/auto_scheduler/evaluator.h>
#include <tiramisu/auto_scheduler/search_method.h>
#include "function_blur_LARGE_wrapper.h"

using namespace tiramisu;

int main(int argc, char **argv)
{
    tiramisu::init("function_blur_LARGE");
    
    // iterators
    var i00("i00", 0, 5) , i01("i01", 0, 514 - 2) , i02("i02", 0, 1026 - 2) ;
    
    // computations
    input input00("input00", {i00, i01, i02}, p_int32);

    computation comp01("comp01", {i00, i01, i02}, input00(i00, i01, i02) + input00(i00, i01 + 1, i02) + input00(i00, i01 + 2, i02) + input00(i00, i01, i02 + 1) + input00(i00, i01 + 1, i02 + 1) + input00(i00, i01 + 2, i02 + 1) + input00(i00, i01, i02 + 2) + input00(i00, i01 + 1, i02 + 2) + input00(i00, i01 + 2, i02 + 2));
    
    // buffers
    buffer buf01("buf01", {5, 514, 1026}, p_int32, a_output);
    buffer buf00("buf00", {5, 514, 1026}, p_int32, a_input);

    comp01.store_in(&buf01);
    input00.store_in(&buf00);
    

    
    tiramisu::codegen({&buf01, &buf00}, "./function_blur_LARGE.o");
    
    return 0;
}
