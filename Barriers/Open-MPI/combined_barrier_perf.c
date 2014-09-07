#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include "mpi.h"
#include "math.h"
#include <stdbool.h>
#include <omp.h>
#include <sys/time.h>

bool localsense = true;
bool sense = true;


int n = 2;
int d = 2;

#define N 2
#define ITER 8000

static int leaves = 0;
typedef struct node {
                      int k;
                      int count; 
                      bool sense;
                      struct node *parent;
} NODE;

NODE *leaf[N]; 

int mysense = true; 
NODE mynode;

#pragma omp threadprivate(mysense, mynode)

void Build(NODE *parent, int height)
{
         int j;
      
         if (height == 0) {
                            leaf[leaves++] = parent; 
                          }
        else {
                          for (j = 0; j < d; j++) 
                          {
                        
                          NODE* child = (NODE*)malloc(sizeof(NODE)); 
                          child->count = d;
                          child->k = d;
                          child->parent = parent;
                          child->sense = false;
                          Build(child, height-1);
                          } 
            }
}

void InitializeBarrier() 
{
        int height = 0;
        if(n == 6) n = 8; 
        while (n > 1){
                         height++; 
                         n = n/2;
                      }
                      NODE* root = (NODE*)malloc(sizeof(NODE));
                      root->count = d;
                      root->k = d;
                      root->parent = NULL;
                      root->sense = false;
                      Build(root, height-1);
}

void wait(NODE **nd)
{
                      int position;
                    
                      #pragma omp critical
                      {
                         (*nd)->count--;
                         position = (*nd)->count;  
                      }
                              
                      if(position == 0)
                     { 
                         
                        if((*nd)->parent != NULL) wait(&((*nd)->parent));

                        (*nd)->count = d;
                        (*nd)->sense = mysense;

                   
                     }
                      else
                      while ((*nd)->sense != mysense);


}

NODE getfromBarrier()
{
    int index = omp_get_thread_num()/2;
    return (*leaf[index]);

}

int main(int argc, char **argv)
{
  int id, num_processes, count, i, iter, proc0count, tag = 1;
  struct timeval tv, tv1;
  time_t finish;
 
  MPI_Status status;
  MPI_Request request;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  InitializeBarrier();
  
  gettimeofday(&tv, NULL);

  for(iter = 0;iter < ITER; iter++){

  proc0count = count = num_processes;

  localsense = !localsense;

 
 #pragma omp parallel num_threads(N) shared(leaf)
    {
     mynode = getfromBarrier();
     wait(&leaf[omp_get_thread_num()/d]);
     mysense = !mysense;
  
    }
  
  proc0count--; 

  if(id != 0)
  {
   MPI_Isend(&tag, 1, MPI_INT, 0, id, MPI_COMM_WORLD, &request);
   MPI_Wait(&request, &status);
  }
  else
  { 
    for(i = 0; i < num_processes - 1; i++){
    MPI_Recv(&tag, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);  
    proc0count --;}
  }

  if(proc0count == 0)
  {
     count = num_processes;
     sense = localsense;

     for(i = 1;i < num_processes; i++){
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

  printf("%ld \n", tv.tv_sec * 1000000 + tv.tv_usec);
  fflush(stdout);
  printf("Finish : %ld\n ", finish*1000000 + tv1.tv_usec);

  MPI_Finalize();
  return 0;
}

