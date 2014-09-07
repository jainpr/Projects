Compilation of the OpenMP Barrier:

1> Type make
2> To run the Barrier: ./omp_helloc



Compilaion of the MPI Barrier:

To compile the file, run 'make'
This will produce mpi_barriers.a library file, centralized_main executable for centralized barrier, and mcs_main executable for mcs barrier.

Run as follows for a sample run of the barriers:
mpirun -n <number of processes> centralized_main
mpirun -n <number of processes> mcs_main

If you write your own file using any of the barriers. compile it as
mpicc -o output_exec <yourfile.c> mpi_barriers.a

and run as

mpirun -n <number of processes> output_exec




Compilation of the Combined Barrier:

1) Type: make

Run:

2) #mpirun –np (number_of_processors) –hostfile (hosts_to_run_on) barriername



To compile the Combined Barrier on the Jinx Cluster:
/opt/openmpi-1.4.3-gcc44/bin/mpicc -fopenmp combined_barrier.c -o <exec-name>

To run: 
/opt/openmpi-1.4.3-gcc44/bin/mpirun <exec-name>



Files Provided:

The files which have "_perf" in their name are the test harnesses while the files having barriers as their name are the actual implementations. The perf files have parameters such as number of iterations and number of threads which can be varied in order to get the different results.

For MPI and the combined barrier: We have used gettimeofday function
For OpenMP:  We have used omp_get-wtime