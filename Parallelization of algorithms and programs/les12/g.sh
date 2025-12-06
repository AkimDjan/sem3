#!/bin/bash
#PBS -l walltime=00:01:30,nodes=3:ppn=3
#PBS -N example_job
#PBS -q batch
cd $PBS_O_WORKDIR
export OMP_NUM_THREADS=$PBS_NUM_PPN
mpirun  --hostfile $PBS_NODEFILE -pernode -x OMP_NUM_THREADS -np $PBS_NUM_NODES ./a.out 10000