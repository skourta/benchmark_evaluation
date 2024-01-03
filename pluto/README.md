# Pluto Execution

This directory contains the scripts and configuration files to run Pluto. It was made for the Jubail cluster, but it can be easily adapted to other clusters.

## Configuration

There are two configuration types to run Pluto: the cluster configuration and the script configuration.

### Cluster configuration

The cluster configuration is defined in the header of the running script. It contains the following variables:

- `#SBATCH -p compute` - the partition to run the job
- `#SBATCH --exclude=dn430` - the nodes to exclude
- `#SBATCH --nodes=1` - the number of nodes to use
- `#SBATCH --exclusive` - the nodes are exclusive to the job
- `#SBATCH --tasks-per-node=1` - the number of tasks per node
- `#SBATCH --cpus-per-task=28` - the number of CPUs per task
- `#SBATCH -t 7-0:00:00` - the maximum time to run the job
- `#SBATCH -o outputs/job.%J.out` - the output file
- `#SBATCH -e outputs/job.%J.err` - the error file
- `#SBATCH --mem=102G` - the memory per node
- `#SBATCH --reservation=c2` - the reservation to use

### Script configuration

The configuration of the script consists of setting the environment variables inside the script. The variables are:

- `CXX` - the C++ compiler
- `CC` - the C compiler
- `CONDA_DIR` - the directory of the conda installation
- `CONDA_ENV` - the name of the conda environment
- `PLUTO_ROOT` - the directory of the Pluto benchmarks
- `NBR_RUNS` - the number of runs to perform


Optionally, the following variables can be changed:
- `WORKDIR_DIR` - the directory to use while compiling and running the benchmarks
- `RESULTS_DIR` - the directory to store the results

## Running Pluto

To run Pluto, simply execute the script `run_pluto.sh`:

```bash
./run_pluto.sh
```

To run Pluto on the cluster (Jubail), submit the job `run_pluto.sh`:

```bash
sbatch run_pluto.sh
```

## Executing the original benchmarks without transformations along with the transformed benchmarks 

To execute the benchmarks without transformations, simply pass the argument `1` to the script:

```bash
./run_pluto.sh 1
```

Or schedule the job with the argument `1`:

```bash
sbatch run_pluto.sh 1
```




