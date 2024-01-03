#include <tiramisu/tiramisu.h>
#include <tiramisu/auto_scheduler/evaluator.h>
#include <tiramisu/auto_scheduler/search_method.h>
#include "function_symm_MEDIUM_wrapper.h"

using namespace tiramisu;

int main(int argc, char **argv)
{
    tiramisu::init("function_symm_MEDIUM");

    // -------------------------------------------------------
    // Layer I
    // ------------------------------------------------------- 
    
    //Iteration variables    
    var i("i", 0, 200), j("j", 0, 240), k("k", 0, 200);
    

    //inputs
    input A("A", {i, k}, p_float64);
    input B("B", {i, j}, p_float64);
    input C("C", {i, j}, p_float64);
    input temp("temp", {}, p_float64);


    //Computations
    computation temp_init("{temp_init[i,j]: 0<=i<200 and 0<=j<240}", expr(), true, p_float64, global::get_implicit_function());
    temp_init.set_expression(0.0);
    computation temp_comp("{temp_comp[i,j,k]: 0<=i<200 and 0<=j<240 and 0<=k<i}", expr(), true, p_float64, global::get_implicit_function());
    temp_comp.set_expression(temp(0)+A(i,k)*B(k,j));
    computation C_r("{C_r[i,j,k]: 0<=i<200 and 0<=j<240 and 0<=k<i}", expr(), true, p_float64, global::get_implicit_function());
    C_r.set_expression(C(k,j) + B(i,j)*A(i,k)*1.5);
    computation C_out("{C_out[i,j]: 0<=i<200 and 0<=j<240}", expr(), true, p_float64, global::get_implicit_function());
    C_out.set_expression(C(i, j)*1.2 + B(i, j)*A(i, i)*1.5 +  temp(0)*1.5);
    
    
    // -------------------------------------------------------
    // Layer II
    // -------------------------------------------------------
    temp_init.then(C_r, j)
             .then(temp_comp, k)
             .then(C_out, j);

    // -------------------------------------------------------
    // Layer III
    // -------------------------------------------------------
    //Input Buffers
    buffer b_A("b_A", {200,200}, p_float64, a_input);
    buffer b_B("b_B", {200,240}, p_float64, a_input);
    buffer b_C("b_C", {200,240}, p_float64, a_output);
    buffer b_temp("b_temp", {1}, p_float64, a_temporary);

    //Store inputs
    A.store_in(&b_A);
    B.store_in(&b_B);
    C.store_in(&b_C);
    temp.store_in(&b_temp);
    

    //Store computations
    temp_init.store_in(&b_temp, {});
    temp_comp.store_in(&b_temp, {});
    C_r.store_in(&b_C, {k,j});
    C_out.store_in(&b_C);

    // -------------------------------------------------------
    // Code Generation
    // -------------------------------------------------------
    tiramisu::codegen({&b_A, &b_B, &b_C}, "function_symm_MEDIUM.o");

    return 0;
}

