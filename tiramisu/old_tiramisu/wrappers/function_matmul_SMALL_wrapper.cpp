#include "Halide.h"
#include "function_matmul_SMALL_wrapper.h"
#include "tiramisu/utils.h"
#include <iostream>
#include <time.h>
#include <fstream>
#include <chrono>

using namespace std::chrono;
using namespace std;      

int main(int, char **argv)
{
    double *c_b_A = (double*)malloc(128*64*sizeof(double));
	parallel_init_buffer(c_b_A,128*64, (double)2);
	Halide::Buffer<double> b_A(c_b_A, 128,64);

	double *c_b_B = (double*)malloc(96*128*sizeof(double));
	parallel_init_buffer(c_b_B,96*128, (double)19);
	Halide::Buffer<double> b_B(c_b_B, 96,128);

	double *c_b_out = (double*)malloc(96*64*sizeof(double));
	parallel_init_buffer(c_b_out,96*64, (double)29);
	Halide::Buffer<double> b_out(c_b_out, 96,64);

	int nb_exec = get_nb_exec();

	for (int i = 0; i < nb_exec; i++) 
	{  
		auto begin = std::chrono::high_resolution_clock::now(); 
		function_matmul_SMALL(b_A.raw_buffer(),b_B.raw_buffer(),b_out.raw_buffer());
		auto end = std::chrono::high_resolution_clock::now(); 

		std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() / (double)1000000 << " " << std::flush; 
	}
		std::cout << std::endl;

	return 0; 
}