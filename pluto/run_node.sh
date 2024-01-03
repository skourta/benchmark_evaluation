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
# if [ $# -eq 0 ]; then
#     echo "No arguments supplied"
#     exit 1
# fi
# check if the user provided a 2nd argument (Execute unoptimized or not)
if [ $# -eq 1 ]; then
    EXECUTE_UNOPTIMIZED=$1
else
    EXECUTE_UNOPTIMIZED=0
fi

CXX=c++
GXX=g++
CC=gcc
CONDA_DIR=/share/apps/NYUAD5/miniconda/3-4.11.0
CONDA_ENV="/home/sk10691/.conda/envs/new-main"
PLUTO_ROOT="/scratch/sk10691/workspace/benchmarks/pipelines/pluto/benchmarks"

NBR_RUNS=30

. $CONDA_DIR/bin/activate $CONDA_ENV

echo "Running on $(hostname)"
echo "Running on $(date)"
echo "Current working directory is $(pwd)"
echo "SLURM_JOBID=$SLURM_JOBID"
echo "SLURM_JOB_NODELIST=$SLURM_JOB_NODELIST"
echo "Benchmarks to execute: ${PLUTO_ROOT}/benchmarks"
# print the list of files to be executed
echo "Files to execute: $(ls $PLUTO_ROOT/benchmarks)"
echo "Execute unoptimized: $EXECUTE_UNOPTIMIZED"
echo "Conda ENV: $CONDA_PREFIX"
echo "Printing the CPU info"
echo
lscpu
echo
echo
echo "--------------------------------------------------"
echo
echo



WORKDIR_DIR="workdir_pluto"
RESULTS_DIR="results_pluto"
rm -fr $WORKDIR_DIR
rm -fr $RESULTS_DIR

mkdir $WORKDIR_DIR
mkdir $RESULTS_DIR

for f in $PLUTO_ROOT/transformed/*.c; do

    BASE_NAME=$(basename $f $suffix)
    # get the function name
    FUNC_NAME=$(echo $BASE_NAME | cut -d "_" -f1)
    FUNC_SIZE=$(echo $BASE_NAME | cut -d "_" -f2)
    echo "Running ${FUNC_NAME} Size ${FUNC_SIZE} ..."

    # go to the WORKDIR_DIR
    cd $WORKDIR_DIR

    # compile the c file
    gcc -O3 -I ${PLUTO_ROOT}/utilities -I ${PLUTO_ROOT}/benchmarks/${FUNC_NAME} ${PLUTO_ROOT}/utilities/polybench.c $f -DPOLYBENCH_TIME -D${FUNC_SIZE}_DATASET -o ${FUNC_NAME}_${FUNC_SIZE}_m_pluto_old -lm -fopenmp

    # if EXECUTE_UNOPTIMIZED is set to 1, then compile the unoptimized version of the file
    if [ $EXECUTE_UNOPTIMIZED -eq 1 ]; then
        gcc -O3 -I ${PLUTO_ROOT}/utilities -I ${PLUTO_ROOT}/benchmarks/${FUNC_NAME} ${PLUTO_ROOT}/utilities/polybench.c ${PLUTO_ROOT}/benchmarks/${FUNC_NAME}/${FUNC_NAME}.c -DPOLYBENCH_TIME -D${FUNC_SIZE}_DATASET -o ${FUNC_NAME}_${FUNC_SIZE}_m_orig -lm -fopenmp
    fi

    # empty the results file before running the benchmark
    echo "" >../results_pluto/${FUNC_NAME}_${FUNC_SIZE}_m_pluto_old.txt

    # Run the compiled file for NBR_RUNS times
    echo "Executing ${FUNC_NAME}_${FUNC_SIZE} TRANSFORMED ..."
    for ((i = 1; i <= $NBR_RUNS; i++)); do
        ./${FUNC_NAME}_${FUNC_SIZE}_m_pluto_old | tee -a ../results_pluto/${FUNC_NAME}_${FUNC_SIZE}_m_pluto_old.txt
    done

    # if EXECUTE_UNOPTIMIZED is set to 1, then run the unoptimized version of the file
    if [ $EXECUTE_UNOPTIMIZED -eq 1 ]; then
        # empty the results file before running the benchmark
        echo "" >../results_pluto/${FUNC_NAME}_${FUNC_SIZE}_m_orig.txt

        # Run the compiled file for NBR_RUNS times
        echo "Executing ${FUNC_NAME}_${FUNC_SIZE} ORIGINAL ..."
        for ((i = 1; i <= $NBR_RUNS; i++)); do
            ./${FUNC_NAME}_${FUNC_SIZE}_m_orig | tee -a ../results_pluto/${FUNC_NAME}_${FUNC_SIZE}_m_orig.txt
        done
    fi

    # remove the files from the WORKDIR_DIR
    rm ${FUNC_NAME}_${FUNC_SIZE}_m_pluto_old
    if [ $EXECUTE_UNOPTIMIZED -eq 1 ]; then
        rm ${FUNC_NAME}_${FUNC_SIZE}_m_orig
    fi

    # if EXECUTE_UNOPTIMIZED is set to 1 print "execute unoptimized" else print "execute optimized"
    if [ $EXECUTE_UNOPTIMIZED -eq 1 ]; then
        printf "Done running ${FUNC_NAME}_${FUNC_SIZE}! Done: $(($(ls ../$RESULTS_DIR | wc -l) / 2))/$(($(ls $PLUTO_ROOT/transformed | wc -l)))\n\n"

    else
        printf "Done running ${FUNC_NAME}_${FUNC_SIZE}! Done: $(ls ../$RESULTS_DIR | wc -l)/$(($(ls $PLUTO_ROOT/transformed | wc -l)))\n\n"

    fi
    # go back to the parent directory
    cd ..
done

rm -rf $WORKDIR_DIR

# # copy log of the scrip to the results directory
cp ../outputs/job.${SLURM_JOBID}.out $RESULTS_DIR
cp ../outputs/job.${SLURM_JOBID}.err $RESULTS_DIR
# python get_results.py --schedules=$PLUTO_ROOT
