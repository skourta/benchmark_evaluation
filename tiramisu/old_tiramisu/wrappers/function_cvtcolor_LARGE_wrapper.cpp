#include "Halide.h"
#include "function_cvtcolor_LARGE_wrapper.h"
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
    Halide::Buffer<double> buf02(1538, 514);

    double *c_buf00 = (double*)malloc(5 * 1538 * 514 * sizeof(double));
    parallel_init_buffer(c_buf00, 5 * 1538 * 514,  (double)41);
    Halide::Buffer<double> buf00(c_buf00, 5, 1538, 514);

    double *c_buf01 = (double*)malloc(5 * sizeof(double));
    parallel_init_buffer(c_buf01, 5,  (double)82);
    Halide::Buffer<double> buf01(c_buf01, 5);

    int nb_exec = get_nb_exec();

	for (int i = 0; i < nb_exec; i++) 
	{  
		auto begin = std::chrono::high_resolution_clock::now(); 
		function_cvtcolor_LARGE(buf02.raw_buffer(), buf00.raw_buffer(), buf01.raw_buffer());
		auto end = std::chrono::high_resolution_clock::now(); 

		std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() / (double)1000000 << " " << std::flush; 
	}
		std::cout << std::endl;
    
    return 0;
}