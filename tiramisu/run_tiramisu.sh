#!/bin/bash
#SBATCH -p compute
#SBATCH --exclude=dn430
#SBATCH --nodes=1
#SBATCH --exclusive
#SBATCH --tasks-per-node=1
#SBATCH --cpus-per-task=28
#SBATCH -t 7-0:00:00
#SBATCH -o outputs/job.%J.out
#SBATCH -e outputs/job.%J.err
#SBATCH --mem=102G
#SBATCH --reservation=c2

# check if the user provided the name of the benchmark folder
if [ $# -eq 0 ]; then
    echo "No arguments supplied"
    exit 1
fi
# check if the user provided a 2nd argument (Execute unoptimized or not)
if [ $# -eq 2 ]; then
    EXECUTE_UNOPTIMIZED=$2
else
    EXECUTE_UNOPTIMIZED=0
fi

echo "Running on $(hostname)"
echo "Running on $(date)"
echo "Current working directory is $(pwd)"
echo "SLURM_JOBID=$SLURM_JOBID"
echo "SLURM_JOB_NODELIST=$SLURM_JOB_NODELIST"
echo "Benchmarks to execute: $1"
# print the list of files to be executed
echo "Files to execute: $(ls $1)"
echo "Execute unoptimized: $EXECUTE_UNOPTIMIZED"
echo "Printing the CPU info"
echo
lscpu
echo
echo
echo "--------------------------------------------------"
echo
echo

TIRAMISU_ROOT=/home/sk10691/tiramisu_new
CXX=c++
GXX=g++
CC=gcc

CONDA_DIR=/share/apps/NYUAD5/miniconda/3-4.11.0
CONDA_ENV=/home/sk10691/.conda/envs/new-main
LD_LIBRARY_PATH=:$CONDA_ENV/lib:${TIRAMISU_ROOT}/3rdParty/Halide/install/lib64:${TIRAMISU_ROOT}/3rdParty/llvm/build/lib:$TIRAMISU_ROOT/3rdParty/isl/build/lib/:


. $CONDA_DIR/bin/activate $CONDA_ENV
export LD_LIBRARY_PATH

WRAPPERS=/scratch/sk10691/workspace/benchmarks/pipelines/tiramisu/wrappers_mpi
GENERATORS=/scratch/sk10691/workspace/benchmarks/pipelines/tiramisu/generators
suffix=".cpp"

export MAX_RUNS=30
export NB_EXEC=30


BENCHMARKS_PATH=$1

cd $BENCHMARKS_PATH

WORKDIR_DIR="workdir"
RESULTS_DIR="results"

mkdir $WORKDIR_DIR
mkdir $RESULTS_DIR

for f in ./*.cpp; do
    # get the function name
    FUNC_NAME=$(basename $f $suffix)

    echo "Running ${FUNC_NAME} ..."

    # copy the files to the WORKDIR_DIR
    cp ./${FUNC_NAME}${suffix} $WORKDIR_DIR
    cp $WRAPPERS/${FUNC_NAME}_wrapper* $WORKDIR_DIR
    if [ $EXECUTE_UNOPTIMIZED -eq 1 ]; then
        cp $GENERATORS/${FUNC_NAME}_generator.cpp $WORKDIR_DIR/${FUNC_NAME}_unoptimized.cpp
    fi

    # go to the WORKDIR_DIR
    cd $WORKDIR_DIR

    # compile the generator of the schedule and run it
    ${CXX} -I${TIRAMISU_ROOT}/3rdParty/Halide/install/include -I${TIRAMISU_ROOT}/include -I${TIRAMISU_ROOT}/3rdParty/isl/include -Wl,--no-as-needed -ldl -g -fno-rtti -lpthread -fopenmp -std=c++17 -O0 -o ${FUNC_NAME}_generator.cpp.o -c ${FUNC_NAME}.cpp

    ${CXX} -Wl,--no-as-needed -ldl -g -fno-rtti -lpthread -fopenmp -std=c++17 -O0 ${FUNC_NAME}_generator.cpp.o -o ./${FUNC_NAME}_generator -L${TIRAMISU_ROOT}/build -L${TIRAMISU_ROOT}/3rdParty/Halide/install/lib64 -L${TIRAMISU_ROOT}/3rdParty/isl/build/lib -Wl,-rpath,${TIRAMISU_ROOT}/build:${TIRAMISU_ROOT}/3rdParty/Halide/install/lib64:${TIRAMISU_ROOT}/3rdParty/isl/build/lib -ltiramisu -ltiramisu_auto_scheduler -lHalide -lisl

    ./${FUNC_NAME}_generator

    # create shared object from the generated object file
    ${CXX} -shared -o ${FUNC_NAME}.o.so ${FUNC_NAME}.o

    # compile the wrapper
    ${CXX} -std=c++17 -fno-rtti -I${TIRAMISU_ROOT}/include -I${TIRAMISU_ROOT}/3rdParty/Halide/install/include -I${TIRAMISU_ROOT}/3rdParty/isl/include/ -I${TIRAMISU_ROOT}/benchmarks -L${TIRAMISU_ROOT}/build -L${TIRAMISU_ROOT}/3rdParty/Halide/install/lib64/ -L${TIRAMISU_ROOT}/3rdParty/isl/build/lib -o ${FUNC_NAME}_wrapper -ltiramisu -lHalide -ldl -lpthread -fopenmp -lm -Wl,-rpath,${TIRAMISU_ROOT}/build ./${FUNC_NAME}_wrapper.cpp ./${FUNC_NAME}.o.so -ltiramisu -lHalide -ldl -lpthread -fopenmp -lm -lisl

    # Compile the unoptimized version
    if [ $EXECUTE_UNOPTIMIZED -eq 1 ]; then
        # remove ${FUNC_NAME}.o of the schedule from workdir
        rm ${FUNC_NAME}.o

        ${CXX} -I${TIRAMISU_ROOT}/3rdParty/Halide/install/include -I${TIRAMISU_ROOT}/include -I${TIRAMISU_ROOT}/3rdParty/isl/include -Wl,--no-as-needed -ldl -g -fno-rtti -lpthread -fopenmp -std=c++17 -O0 -o ${FUNC_NAME}_unoptimized_generator.o -c ${FUNC_NAME}_unoptimized.cpp

        ${CXX} -Wl,--no-as-needed -ldl -g -fno-rtti -lpthread -fopenmp -std=c++17 -O0 ${FUNC_NAME}_unoptimized_generator.o -o ./${FUNC_NAME}_unoptimized_generator -L${TIRAMISU_ROOT}/build -L${TIRAMISU_ROOT}/3rdParty/Halide/install/lib64 -L${TIRAMISU_ROOT}/3rdParty/isl/build/lib -Wl,-rpath,${TIRAMISU_ROOT}/build:${TIRAMISU_ROOT}/3rdParty/Halide/install/lib64:${TIRAMISU_ROOT}/3rdParty/isl/build/lib -ltiramisu -ltiramisu_auto_scheduler -lHalide -lisl

        ./${FUNC_NAME}_unoptimized_generator

        # create shared object from the generated object file
        ${CXX} -shared -o ${FUNC_NAME}_unoptimized.o.so ${FUNC_NAME}.o

        # compile the wrapper
        ${CXX} -std=c++17 -fno-rtti -I${TIRAMISU_ROOT}/include -I${TIRAMISU_ROOT}/3rdParty/Halide/install/include -I${TIRAMISU_ROOT}/3rdParty/isl/include/ -I${TIRAMISU_ROOT}/benchmarks -L${TIRAMISU_ROOT}/build -L${TIRAMISU_ROOT}/3rdParty/Halide/install/lib64/ -L${TIRAMISU_ROOT}/3rdParty/isl/build/lib -o ${FUNC_NAME}_unoptimized_wrapper -ltiramisu -lHalide -ldl -lpthread -fopenmp -lm -Wl,-rpath,${TIRAMISU_ROOT}/build ./${FUNC_NAME}_wrapper.cpp ./${FUNC_NAME}_unoptimized.o.so -ltiramisu -lHalide -ldl -lpthread -fopenmp -lm -lisl
    fi

    # Running the compiled wrappers
    printf "Running Schedule of ${FUNC_NAME} ...\n"
    ./${FUNC_NAME}_wrapper | tee ../results/${FUNC_NAME}.txt

    if [ $EXECUTE_UNOPTIMIZED -eq 1 ]; then
        printf "Running Unoptimized Schedule of ${FUNC_NAME} ...\n"
        ./${FUNC_NAME}_unoptimized_wrapper | tee ../results/${FUNC_NAME}_unoptimized.txt
    fi

    # remove the files from the WORKDIR_DIR
    rm -rf ${FUNC_NAME}_generator.cpp.o
    rm -rf ${FUNC_NAME}_generator
    rm -rf ${FUNC_NAME}.o
    rm -rf ${FUNC_NAME}.o.so
    rm -rf ${FUNC_NAME}_wrapper
    if [ $EXECUTE_UNOPTIMIZED -eq 1 ]; then
        rm -rf ${FUNC_NAME}_unoptimized_generator.cpp.o
        rm -rf ${FUNC_NAME}_unoptimized_generator
        rm -rf ${FUNC_NAME}.o
        rm -rf ${FUNC_NAME}_unoptimized.o.so
        rm -rf ${FUNC_NAME}_unoptimized_wrapper
    fi

    printf "Done running ${FUNC_NAME}! Done: $(ls ../results | wc -l)/$(($(ls .. | wc -l) - 2))\n\n"

    # go back to the parent directory
    cd ..
done

rm -rf $WORKDIR_DIR

# copy log of the scrip to the results directory
cp ../outputs/job.${SLURM_JOBID}.out $RESULTS_DIR
cp ../outputs/job.${SLURM_JOBID}.err $RESULTS_DIR