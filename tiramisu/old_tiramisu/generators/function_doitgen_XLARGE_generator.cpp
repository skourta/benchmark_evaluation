#include <tiramisu/tiramisu.h>
#include <tiramisu/auto_scheduler/evaluator.h>
#include <tiramisu/auto_scheduler/search_method.h>
#include "function_doitgen_XLARGE_wrapper.h"

using namespace tiramisu;

int main(int argc, char **argv)
{
    tiramisu::init("function_doitgen_XLARGE");
//     constant c00("c00", 3), c01("c01", 768), c02("c02", 384);

    var i00("i00", 0, 220) , i01("i01", 0, 270) , i02("i02", 0, 250) , i03("i03", 0, 250) ;

    input input00("input00", {i00, i01, i03}, p_int32);
    input input01("input01", {i02, i03}, p_int32);

    computation comp03("comp03", {i00, i01, i02, i03}, p_int32);
    comp03.set_expression(comp03(i00, i01, i02, i03) + input00(i00, i01, i03) * input01(i02, i03));



    buffer buf03("buf03", {220, 270, 250}, p_int32, a_output);
    buffer buf00("buf00", {220, 270, 250}, p_int32, a_input);
    buffer buf01("buf01", {250, 250}, p_int32, a_input);

    comp03.store_in(&buf03, {i00, i01, i02});
    input00.store_in(&buf00);
    input01.store_in(&buf01);
    tiramisu::codegen({&buf03, &buf00, &buf01}, "./function_doitgen_XLARGE.o");
    
    return 0;
}
