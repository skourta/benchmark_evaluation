#include <tiramisu/tiramisu.h>
#include <tiramisu/auto_scheduler/evaluator.h>
#include <tiramisu/auto_scheduler/search_method.h>
#include "function_conv_relu_MEDIUM_wrapper.h"

using namespace tiramisu;
int main(int argc, char **argv)
{
    tiramisu::init("function_conv_relu_MEDIUM");
    // constant c00("c00", 3), c01("c01", 256), c02("c02", 2048);
    // function_conv_relu_64x6x320x96x3x4x9
    // iterators
    int c00(64), c01(10), c02(96), c03(160), c04(3), c05(4), c06(10), c07(99), c08(169);

    var i00("i00", 0, c00), i01("i01", 0, c01), i02("i02", 0, c02), i03("i03", 0, c03), i04("i04", 0, c04), i05("i05", 0, c05), i06("i06", 0, c06), i07("i07", 0, c07), i08("i08", 0, c08);

    input input00("input00", {i01}, p_int32);
    input input02("input02", {i00, i04, i07, i08}, p_int32);
    input input03("input03", {i01, i04, i05, i06}, p_int32);

    computation comp01("comp01", {i00, i01, i02, i03}, input00(i01));
    computation comp04("comp04", {i00, i01, i02, i03, i04, i05, i06}, p_int32);
    computation comp05("comp05", {i00, i01, i02, i03}, expr(o_max, comp04(i00, i01, i02, i03, 0, 0, 0), 0));
    comp04.set_expression(comp04(i00, i01, i02, i03, i04, i05, i06) + input02(i00, i04, i02 + i05, i03 + i06) * input03(i01, i04, i05, i06));

    comp01.then(comp04, i03)
        .then(comp05, i03);

    buffer buf01("buf01", {64, 10, 96, 160}, p_int32, a_output);
    buffer buf05("buf05", {64, 10, 96, 160}, p_int32, a_output);
    buffer buf00("buf00", {10}, p_int32, a_input);
    buffer buf02("buf02", {64, 3, 99, 169}, p_int32, a_input);
    buffer buf03("buf03", {10, 3, 4, 10}, p_int32, a_input);

    comp01.store_in(&buf01);
    comp05.store_in(&buf05);
    input00.store_in(&buf00);
    input02.store_in(&buf02);
    input03.store_in(&buf03);
    comp04.store_in(&buf01, {i00, i01, i02, i03});

    comp01.interchange(1, 2);
    comp05.interchange(1, 2);
    comp04.interchange(1, 2);

    comp01.tag_parallel_level(0);

    tiramisu::codegen({&buf01, &buf05, &buf00, &buf02, &buf03}, "./function_conv_relu_MEDIUM.o");

    return 0;
}
