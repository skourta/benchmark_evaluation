#include <omp.h>
#include <math.h>
#define ceild(n,d)  ceil(((double)(n))/((double)(d)))
#define floord(n,d) floor(((double)(n))/((double)(d)))
#define max(x,y)    ((x) > (y)? (x) : (y))
#define min(x,y)    ((x) < (y)? (x) : (y))

/**
 * This version is stamped on May 10, 2016
 *
 * Contact:
 *   Louis-Noel Pouchet <pouchet.ohio-state.edu>
 *   Tomofumi Yuki <tomofumi.yuki.fr>
 *
 * Web address: http://polybench.sourceforge.net
 */
/* fdtd-2d.c: this file is part of PolyBench/C */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

/* Include polybench common header. */
#include <polybench.h>

/* Include benchmark-specific header. */
#include "fdtd-2d.h"


/* Array initialization. */
static
void init_array (int tmax,
		 int nx,
		 int ny,
		 DATA_TYPE POLYBENCH_2D(ex,NX,NY,nx,ny),
		 DATA_TYPE POLYBENCH_2D(ey,NX,NY,nx,ny),
		 DATA_TYPE POLYBENCH_2D(hz,NX,NY,nx,ny),
		 DATA_TYPE POLYBENCH_1D(_fict_,TMAX,tmax))
{
  int i, j;

  for (i = 0; i < tmax; i++)
    _fict_[i] = (DATA_TYPE) i;
  for (i = 0; i < nx; i++)
    for (j = 0; j < ny; j++)
      {
	ex[i][j] = ((DATA_TYPE) i*(j+1)) / nx;
	ey[i][j] = ((DATA_TYPE) i*(j+2)) / ny;
	hz[i][j] = ((DATA_TYPE) i*(j+3)) / nx;
      }
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int nx,
		 int ny,
		 DATA_TYPE POLYBENCH_2D(ex,NX,NY,nx,ny),
		 DATA_TYPE POLYBENCH_2D(ey,NX,NY,nx,ny),
		 DATA_TYPE POLYBENCH_2D(hz,NX,NY,nx,ny))
{
  int i, j;

  POLYBENCH_DUMP_START;
  POLYBENCH_DUMP_BEGIN("ex");
  for (i = 0; i < nx; i++)
    for (j = 0; j < ny; j++) {
      if ((i * nx + j) % 20 == 0) fprintf(POLYBENCH_DUMP_TARGET, "\n");
      fprintf(POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, ex[i][j]);
    }
  POLYBENCH_DUMP_END("ex");
  POLYBENCH_DUMP_FINISH;

  POLYBENCH_DUMP_BEGIN("ey");
  for (i = 0; i < nx; i++)
    for (j = 0; j < ny; j++) {
      if ((i * nx + j) % 20 == 0) fprintf(POLYBENCH_DUMP_TARGET, "\n");
      fprintf(POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, ey[i][j]);
    }
  POLYBENCH_DUMP_END("ey");

  POLYBENCH_DUMP_BEGIN("hz");
  for (i = 0; i < nx; i++)
    for (j = 0; j < ny; j++) {
      if ((i * nx + j) % 20 == 0) fprintf(POLYBENCH_DUMP_TARGET, "\n");
      fprintf(POLYBENCH_DUMP_TARGET, DATA_PRINTF_MODIFIER, hz[i][j]);
    }
  POLYBENCH_DUMP_END("hz");
}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_fdtd_2d(int tmax,
		    int nx,
		    int ny,
		    DATA_TYPE POLYBENCH_2D(ex,NX,NY,nx,ny),
		    DATA_TYPE POLYBENCH_2D(ey,NX,NY,nx,ny),
		    DATA_TYPE POLYBENCH_2D(hz,NX,NY,nx,ny),
		    DATA_TYPE POLYBENCH_1D(_fict_,TMAX,tmax))
{
  int t, i, j;

/* Copyright (C) 1991-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/* We do not support C11 <threads.h>.  */
  int t1, t2, t3, t4, t5, t6;
 int lb, ub, lbp, ubp, lb2, ub2;
 register int lbv, ubv;
/* Start of CLooG code */
if ((_PB_NY >= 1) && (_PB_TMAX >= 1)) {
  for (t1=0;t1<=floord(2*_PB_TMAX+_PB_NY-3,32);t1++) {
    lbp=max(ceild(t1,2),ceild(32*t1-_PB_TMAX+1,32));
    ubp=min(min(floord(_PB_TMAX+_PB_NY-2,32),floord(32*t1+_PB_NY+30,64)),t1);
#pragma omp parallel for private(lbv,ubv,t3,t4,t5,t6)
    for (t2=lbp;t2<=ubp;t2++) {
      for (t3=t1-t2;t3<=min(min(floord(32*t1-32*t2+31*_PB_NX,32),floord(32*t1-32*t2+_PB_NX+30,32)),floord(32*t1-32*t2+30*_PB_TMAX+31*_PB_NX-30,992));t3++) {
        if ((_PB_NX >= 2) && (_PB_NY >= 2) && (t1 == 2*t2) && (t1 == 2*t3)) {
          for (t4=16*t1;t4<=min(_PB_TMAX-1,16*t1+30);t4++) {
            if (t1%2 == 0) {
              ey[0][0] = _fict_[t4];;
            }
            for (t6=t4+1;t6<=min(16*t1+31,t4+_PB_NY-1);t6++) {
              if (t1%2 == 0) {
                ey[0][(-t4+t6)] = _fict_[t4];;
              }
              if (t1%2 == 0) {
                ex[0][(-t4+t6)] = ex[0][(-t4+t6)] - SCALAR_VAL(0.5)*(hz[0][(-t4+t6)]-hz[0][(-t4+t6)-1]);;
              }
            }
            for (t5=t4+1;t5<=min(16*t1+31,t4+_PB_NX-1);t5++) {
              if (t1%2 == 0) {
                ey[(-t4+t5)][0] = ey[(-t4+t5)][0] - SCALAR_VAL(0.5)*(hz[(-t4+t5)][0]-hz[(-t4+t5)-1][0]);;
              }
              for (t6=t4+1;t6<=min(16*t1+31,t4+_PB_NY-1);t6++) {
                if (t1%2 == 0) {
                  ey[(-t4+t5)][(-t4+t6)] = ey[(-t4+t5)][(-t4+t6)] - SCALAR_VAL(0.5)*(hz[(-t4+t5)][(-t4+t6)]-hz[(-t4+t5)-1][(-t4+t6)]);;
                }
                if (t1%2 == 0) {
                  ex[(-t4+t5)][(-t4+t6)] = ex[(-t4+t5)][(-t4+t6)] - SCALAR_VAL(0.5)*(hz[(-t4+t5)][(-t4+t6)]-hz[(-t4+t5)][(-t4+t6)-1]);;
                }
                if (t1%2 == 0) {
                  hz[(-t4+t5-1)][(-t4+t6-1)] = hz[(-t4+t5-1)][(-t4+t6-1)] - SCALAR_VAL(0.7)* (ex[(-t4+t5-1)][(-t4+t6-1)+1] - ex[(-t4+t5-1)][(-t4+t6-1)] + ey[(-t4+t5-1)+1][(-t4+t6-1)] - ey[(-t4+t5-1)][(-t4+t6-1)]);;
                }
              }
            }
          }
        }
        if ((_PB_NX >= 2) && (t1 == t2+t3) && (t1 <= 2*t2-1)) {
          for (t4=max(32*t1-32*t2,32*t2-_PB_NY+1);t4<=min(_PB_TMAX-1,32*t1-32*t2+30);t4++) {
            for (t6=32*t2;t6<=min(32*t2+31,t4+_PB_NY-1);t6++) {
              ey[0][(-t4+t6)] = _fict_[t4];;
              ex[0][(-t4+t6)] = ex[0][(-t4+t6)] - SCALAR_VAL(0.5)*(hz[0][(-t4+t6)]-hz[0][(-t4+t6)-1]);;
            }
            for (t5=t4+1;t5<=min(32*t1-32*t2+31,t4+_PB_NX-1);t5++) {
              for (t6=32*t2;t6<=min(32*t2+31,t4+_PB_NY-1);t6++) {
                ey[(-t4+t5)][(-t4+t6)] = ey[(-t4+t5)][(-t4+t6)] - SCALAR_VAL(0.5)*(hz[(-t4+t5)][(-t4+t6)]-hz[(-t4+t5)-1][(-t4+t6)]);;
                ex[(-t4+t5)][(-t4+t6)] = ex[(-t4+t5)][(-t4+t6)] - SCALAR_VAL(0.5)*(hz[(-t4+t5)][(-t4+t6)]-hz[(-t4+t5)][(-t4+t6)-1]);;
                hz[(-t4+t5-1)][(-t4+t6-1)] = hz[(-t4+t5-1)][(-t4+t6-1)] - SCALAR_VAL(0.7)* (ex[(-t4+t5-1)][(-t4+t6-1)+1] - ex[(-t4+t5-1)][(-t4+t6-1)] + ey[(-t4+t5-1)+1][(-t4+t6-1)] - ey[(-t4+t5-1)][(-t4+t6-1)]);;
              }
            }
          }
        }
        if ((_PB_NX >= 2) && (_PB_NY == 1) && (t1 == 2*t2) && (t1 == 2*t3)) {
          for (t4=16*t1;t4<=min(_PB_TMAX-1,16*t1+30);t4++) {
            if (t1%2 == 0) {
              ey[0][0] = _fict_[t4];;
            }
            for (t5=t4+1;t5<=min(16*t1+31,t4+_PB_NX-1);t5++) {
              if (t1%2 == 0) {
                ey[(-t4+t5)][0] = ey[(-t4+t5)][0] - SCALAR_VAL(0.5)*(hz[(-t4+t5)][0]-hz[(-t4+t5)-1][0]);;
              }
            }
          }
        }
        if ((_PB_NX >= 2) && (t1 == t2+t3) && (t1 <= min(floord(32*t2+_PB_TMAX-32,32),2*t2-1))) {
          for (t6=32*t2;t6<=min(32*t2+31,32*t1-32*t2+_PB_NY+30);t6++) {
            ey[0][(-32*t1+32*t2+t6-31)] = _fict_[(32*t1-32*t2+31)];;
            ex[0][(-32*t1+32*t2+t6-31)] = ex[0][(-32*t1+32*t2+t6-31)] - SCALAR_VAL(0.5)*(hz[0][(-32*t1+32*t2+t6-31)]-hz[0][(-32*t1+32*t2+t6-31)-1]);;
          }
        }
        if ((_PB_NX >= 2) && (t1 == 2*t2) && (t1 == 2*t3) && (t1 <= floord(_PB_TMAX-32,16))) {
          if (t1%2 == 0) {
            ey[0][0] = _fict_[(16*t1+31)];;
          }
        }
        if ((_PB_NX == 1) && (_PB_NY >= 2) && (t1 == 2*t2) && (t1 == 2*t3)) {
          for (t4=16*t1;t4<=min(_PB_TMAX-1,16*t1+30);t4++) {
            if (t1%2 == 0) {
              ey[0][0] = _fict_[t4];;
            }
            for (t6=t4+1;t6<=min(16*t1+31,t4+_PB_NY-1);t6++) {
              if (t1%2 == 0) {
                ey[0][(-t4+t6)] = _fict_[t4];;
              }
              if (t1%2 == 0) {
                ex[0][(-t4+t6)] = ex[0][(-t4+t6)] - SCALAR_VAL(0.5)*(hz[0][(-t4+t6)]-hz[0][(-t4+t6)-1]);;
              }
            }
          }
        }
        if ((_PB_NX == 1) && (t1 == t2+t3) && (t1 <= 2*t2-1)) {
          for (t4=max(32*t1-32*t2,32*t2-_PB_NY+1);t4<=min(_PB_TMAX-1,32*t1-32*t2+31);t4++) {
            for (t6=32*t2;t6<=min(32*t2+31,t4+_PB_NY-1);t6++) {
              ey[0][(-t4+t6)] = _fict_[t4];;
              ex[0][(-t4+t6)] = ex[0][(-t4+t6)] - SCALAR_VAL(0.5)*(hz[0][(-t4+t6)]-hz[0][(-t4+t6)-1]);;
            }
          }
        }
        if ((_PB_NX == 1) && (_PB_NY >= 2) && (t1 == 2*t2) && (t1 == 2*t3) && (t1 <= floord(_PB_TMAX-32,16))) {
          if (t1%2 == 0) {
            ey[0][0] = _fict_[(16*t1+31)];;
          }
        }
        if ((_PB_NX == 0) && (_PB_NY >= 2) && (t1 == t2+t3)) {
          for (t4=max(32*t1-32*t2,32*t2-_PB_NY+1);t4<=min(_PB_TMAX-1,32*t1-32*t2+31);t4++) {
            for (t6=max(32*t2,t4);t6<=min(32*t2+31,t4+_PB_NY-1);t6++) {
              ey[0][(-t4+t6)] = _fict_[t4];;
            }
          }
        }
        if ((_PB_NX <= 1) && (_PB_NY == 1) && (t1 == 2*t2) && (t1 == 2*t3)) {
          for (t4=16*t1;t4<=min(_PB_TMAX-1,16*t1+31);t4++) {
            if (t1%2 == 0) {
              ey[0][0] = _fict_[t4];;
            }
          }
        }
        if ((_PB_NY >= 2) && (t1 == 2*t2) && (t1 <= 2*t3-2)) {
          for (t4=max(16*t1,32*t3-_PB_NX+1);t4<=min(_PB_TMAX-1,16*t1+30);t4++) {
            for (t5=32*t3;t5<=min(32*t3+31,t4+_PB_NX-1);t5++) {
              if (t1%2 == 0) {
                ey[(-t4+t5)][0] = ey[(-t4+t5)][0] - SCALAR_VAL(0.5)*(hz[(-t4+t5)][0]-hz[(-t4+t5)-1][0]);;
              }
              for (t6=t4+1;t6<=min(16*t1+31,t4+_PB_NY-1);t6++) {
                if (t1%2 == 0) {
                  ey[(-t4+t5)][(-t4+t6)] = ey[(-t4+t5)][(-t4+t6)] - SCALAR_VAL(0.5)*(hz[(-t4+t5)][(-t4+t6)]-hz[(-t4+t5)-1][(-t4+t6)]);;
                }
                if (t1%2 == 0) {
                  ex[(-t4+t5)][(-t4+t6)] = ex[(-t4+t5)][(-t4+t6)] - SCALAR_VAL(0.5)*(hz[(-t4+t5)][(-t4+t6)]-hz[(-t4+t5)][(-t4+t6)-1]);;
                }
                if (t1%2 == 0) {
                  hz[(-t4+t5-1)][(-t4+t6-1)] = hz[(-t4+t5-1)][(-t4+t6-1)] - SCALAR_VAL(0.7)* (ex[(-t4+t5-1)][(-t4+t6-1)+1] - ex[(-t4+t5-1)][(-t4+t6-1)] + ey[(-t4+t5-1)+1][(-t4+t6-1)] - ey[(-t4+t5-1)][(-t4+t6-1)]);;
                }
              }
            }
          }
        }
        if (t1 <= min(2*t2-1,t2+t3-1)) {
          for (t4=max(max(32*t1-32*t2,32*t2-_PB_NY+1),32*t3-_PB_NX+1);t4<=min(_PB_TMAX-1,32*t1-32*t2+31);t4++) {
            for (t5=32*t3;t5<=min(32*t3+31,t4+_PB_NX-1);t5++) {
              for (t6=32*t2;t6<=min(32*t2+31,t4+_PB_NY-1);t6++) {
                ey[(-t4+t5)][(-t4+t6)] = ey[(-t4+t5)][(-t4+t6)] - SCALAR_VAL(0.5)*(hz[(-t4+t5)][(-t4+t6)]-hz[(-t4+t5)-1][(-t4+t6)]);;
                ex[(-t4+t5)][(-t4+t6)] = ex[(-t4+t5)][(-t4+t6)] - SCALAR_VAL(0.5)*(hz[(-t4+t5)][(-t4+t6)]-hz[(-t4+t5)][(-t4+t6)-1]);;
                hz[(-t4+t5-1)][(-t4+t6-1)] = hz[(-t4+t5-1)][(-t4+t6-1)] - SCALAR_VAL(0.7)* (ex[(-t4+t5-1)][(-t4+t6-1)+1] - ex[(-t4+t5-1)][(-t4+t6-1)] + ey[(-t4+t5-1)+1][(-t4+t6-1)] - ey[(-t4+t5-1)][(-t4+t6-1)]);;
              }
            }
          }
        }
        if ((_PB_NY >= 2) && (t1 == 2*t2) && (t1 <= min(floord(_PB_TMAX-32,16),2*t3-2))) {
          for (t5=32*t3;t5<=min(32*t3+31,16*t1+_PB_NX+30);t5++) {
            if (t1%2 == 0) {
              ey[(-16*t1+t5-31)][0] = ey[(-16*t1+t5-31)][0] - SCALAR_VAL(0.5)*(hz[(-16*t1+t5-31)][0]-hz[(-16*t1+t5-31)-1][0]);;
            }
          }
        }
        if ((_PB_NY == 1) && (t1 == 2*t2) && (t1 <= 2*t3-2)) {
          for (t4=max(16*t1,32*t3-_PB_NX+1);t4<=min(_PB_TMAX-1,16*t1+31);t4++) {
            for (t5=32*t3;t5<=min(32*t3+31,t4+_PB_NX-1);t5++) {
              if (t1%2 == 0) {
                ey[(-t4+t5)][0] = ey[(-t4+t5)][0] - SCALAR_VAL(0.5)*(hz[(-t4+t5)][0]-hz[(-t4+t5)-1][0]);;
              }
            }
          }
        }
      }
      if (_PB_NX <= -1) {
        for (t4=max(32*t1-32*t2,32*t2-_PB_NY+1);t4<=min(_PB_TMAX-1,32*t1-32*t2+31);t4++) {
          for (t6=max(32*t2,t4);t6<=min(32*t2+31,t4+_PB_NY-1);t6++) {
            ey[0][(-t4+t6)] = _fict_[t4];;
          }
        }
      }
    }
  }
}
/* End of CLooG code */
}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int tmax = TMAX;
  int nx = NX;
  int ny = NY;

  /* Variable declaration/allocation. */
  POLYBENCH_2D_ARRAY_DECL(ex,DATA_TYPE,NX,NY,nx,ny);
  POLYBENCH_2D_ARRAY_DECL(ey,DATA_TYPE,NX,NY,nx,ny);
  POLYBENCH_2D_ARRAY_DECL(hz,DATA_TYPE,NX,NY,nx,ny);
  POLYBENCH_1D_ARRAY_DECL(_fict_,DATA_TYPE,TMAX,tmax);

  /* Initialize array(s). */
  init_array (tmax, nx, ny,
	      POLYBENCH_ARRAY(ex),
	      POLYBENCH_ARRAY(ey),
	      POLYBENCH_ARRAY(hz),
	      POLYBENCH_ARRAY(_fict_));

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_fdtd_2d (tmax, nx, ny,
		  POLYBENCH_ARRAY(ex),
		  POLYBENCH_ARRAY(ey),
		  POLYBENCH_ARRAY(hz),
		  POLYBENCH_ARRAY(_fict_));


  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(nx, ny, POLYBENCH_ARRAY(ex),
				    POLYBENCH_ARRAY(ey),
				    POLYBENCH_ARRAY(hz)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(ex);
  POLYBENCH_FREE_ARRAY(ey);
  POLYBENCH_FREE_ARRAY(hz);
  POLYBENCH_FREE_ARRAY(_fict_);

  return 0;
}
