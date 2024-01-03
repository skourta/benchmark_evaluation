#include "Halide.h"
#include "function_doitgen_SMALL_wrapper.h"
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
    Halide::Buffer<double> buf03(25, 30, 20);


    double *c_buf00 = (double*)malloc(25 * 30 * 20 * sizeof(double));
    parallel_init_buffer(c_buf00, 25 * 30 * 20,  (double)100);
    Halide::Buffer<double> buf00(c_buf00, 25, 30, 20);


    double *c_buf01 = (double*)malloc(25 * 25 * sizeof(double));
    parallel_init_buffer(c_buf01, 25 * 25,  (double)39);
    Halide::Buffer<double> buf01(c_buf01, 25, 25);

    
    int nb_exec = get_nb_exec();

	for (int i = 0; i < nb_exec; i++) 
	{  
		auto begin = std::chrono::high_resolution_clock::now(); 
		function_doitgen_SMALL(buf03.raw_buffer(), buf00.raw_buffer(), buf01.raw_buffer());
		auto end = std::chrono::high_resolution_clock::now(); 

		std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() / (double)1000000 << " " << std::flush; 
	}
		std::cout << std::endl;
    
    return 0;
}
