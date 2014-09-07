#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "math.h"
#include <stdbool.h>
#include <sys/time.h>

bool localsense = true;
bool sense = true;

int main(int argc, char **argv)
{
  int id, num_processes, count, i, iter, proc0count;
  double time_start, time_finish, process_time = 0, average_time = 0, total_time = 0;
  struct timeval tv, tv1;
  time_t start, finish, time_us, finish_us;
 
  int tag = 1;
  int my_msg[2];
  MPI_Status status;
  MPI_Request request;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

 // time_start = MPI_Wtime();

  gettimeofday(&tv, NULL);
  start = tv.tv_sec; 
  time_us = tv.tv_usec;
 
  for(iter = 0;iter < 8000; iter++){

  time_start = MPI_Wtime();
  printf("Time is %lu", time_start);

  proc0count = num_processes;

  localsense = !localsense;
  proc0count--;
 

 // printf("The count is %d for rank %d\n", count, id);

  if(id != 0)
  {
   MPI_Isend(&tag, 1, MPI_INT, 0, id, MPI_COMM_WORLD, &request);
   MPI_Wait(&request, &status);
  
 //  printf("I came in ");
  }
  else
  { 
    for(i = 0; i < num_processes - 1; i++){
    MPI_Recv(&tag, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);  
    proc0count --;}
  }
 // printf("The count is %d for rank %d\n", proc0count, id);

  if(proc0count == 0)
  {
     proc0count = num_processes;
  
     sense = localsense;

     for(i = 1;i <= num_processes - 1; i++){
     MPI_Isend(&sense, 1, MPI_INT, i, i, MPI_COMM_WORLD, &request);
     MPI_Wait(&request, &status);}     

   }
  else
     while(sense != localsense){
                                  MPI_Recv(&sense, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status); 
                               }
  }
  gettimeofday(&tv1, NULL);
  finish = tv1.tv_sec;
  finish_us = tv1.tv_usec;

  
  printf("%ld \n", tv.tv_sec * 1000000 + tv.tv_usec);
  fflush(stdout);
  printf("Finish : %ld\n ", finish*1000000 + tv1.tv_usec);
  fflush(stdout);
  printf("Finish in US: %f\n ", finish_us);
  fflush(stdout);
  printf("Total Time is %lu\n\tAverage time is %lu \n", average_time, finish - start);


  MPI_Finalize();
  return 0;
}

