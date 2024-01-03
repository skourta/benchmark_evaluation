#include "Halide.h"
#include "function_conv_relu_MEDIUM_wrapper.h"
#include "tiramisu/utils.h"
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <fstream>
#include <chrono>

#define MAX_RAND 200

using namespace std::chrono;
using namespace std;

int main(int, char **argv)
{
    Halide::Buffer<double> buf01(160, 96, 10, 64);


    Halide::Buffer<double> buf05(160, 96, 10, 64);


    double *c_buf00 = (double*)malloc(10 * sizeof(double));
    parallel_init_buffer(c_buf00, 10,  (double)37);
    Halide::Buffer<double> buf00(c_buf00, 10);


    double *c_buf02 = (double*)malloc(169 * 99 * 3 * 64 * sizeof(double));
    parallel_init_buffer(c_buf02, 169 * 99 * 3 * 64,  (double)87);
    Halide::Buffer<double> buf02(c_buf02, 169, 99, 3, 64);


    double *c_buf03 = (double*)malloc(10 * 4 * 3 * 10 * sizeof(double));
    parallel_init_buffer(c_buf03, 10 * 4 * 3 * 10,  (double)23);
    Halide::Buffer<double> buf03(c_buf03, 10, 4, 3, 10);


    int nb_exec = get_nb_exec();

	for (int i = 0; i < nb_exec; i++) 
	{  
		auto begin = std::chrono::high_resolution_clock::now(); 
		function_conv_relu_MEDIUM(buf01.raw_buffer(), buf05.raw_buffer(), buf00.raw_buffer(), buf02.raw_buffer(), buf03.raw_buffer());
		auto end = std::chrono::high_resolution_clock::now(); 

		std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() / (double)1000000 << " " << std::flush; 
	}
		std::cout << std::endl;
    
    
    return 0;
}
