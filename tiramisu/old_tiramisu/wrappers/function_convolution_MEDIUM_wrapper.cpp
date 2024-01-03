#include "Halide.h"
#include "function_convolution_MEDIUM_wrapper.h"
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
    Halide::Buffer<double> buf01(1024, 1024, 2, 8);

    double *c_buf00 = (double*)malloc(2 * sizeof(double));
    parallel_init_buffer(c_buf00, 2,  (double)72);
    Halide::Buffer<double> buf00(c_buf00, 2);

    double *c_buf02 = (double*)malloc(1026 * 1026 * 3 * 8 * sizeof(double));
    parallel_init_buffer(c_buf02, 1026 * 1026 * 3 * 8,  (double)69);
    Halide::Buffer<double> buf02(c_buf02, 1026, 1026, 3, 8);

    double *c_buf03 = (double*)malloc(3 * 3 * 3 * 2 * sizeof(double));
    parallel_init_buffer(c_buf03, 3 * 3 * 3 * 2,  (double)57);
    Halide::Buffer<double> buf03(c_buf03, 3, 3, 3, 2);
    
    int nb_exec = get_nb_exec();

	for (int i = 0; i < nb_exec; i++) 
	{  
		auto begin = std::chrono::high_resolution_clock::now(); 
		function_convolution_MEDIUM(buf01.raw_buffer(), buf00.raw_buffer(), buf02.raw_buffer(), buf03.raw_buffer());
		auto end = std::chrono::high_resolution_clock::now(); 

		std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() / (double)1000000 << " " << std::flush; 
	}
		std::cout << std::endl;
    
    return 0;
}
