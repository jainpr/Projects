#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

int n = 4;
int d = 2;

#define N 4
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
double time_start, time_finish, thread_time;
#pragma omp threadprivate(mysense, mynode,time_start,time_finish, thread_time)

void create_Barrier(NODE *parent, int height)
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
                          create_Barrier(child, height-1);
                          } 
            }
}

void init_Barrier() 
{
        int height = 0;
        if(n == 6 ) n = 8;
 
        while (n > 1){
                         height++; 
                         n = n/2;
                      }
                      NODE* root = (NODE*)malloc(sizeof(NODE));
                      root->count = d;
                      root->k = d;
                      root->parent = NULL;
                      root->sense = false;
                      create_Barrier(root, height-1);
}

void combining_barrier(NODE **nd)
{
                      int position;

                                       
                      #pragma omp critical
                      {
                         (*nd)->count--;
                         position = (*nd)->count;  
                      }
                     
                          
                      if(position == 0)
                     { 
                        
                        if((*nd)->parent != NULL){

                            wait(&((*nd)->parent));}

                        (*nd)->count = 2;
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

/*Directly Building a tree with 2 nodes/ parent*/
int main()
{
    int i; 
    double total_time = 0, avg_time[n];
    for(i = 0; i < n; i++) avg_time[i] = 0;

    init_Barrier();
    for(i = 0; i < ITER; i++){
    #pragma omp parallel num_threads(N) shared(leaf)
    {
     time_start = omp_get_wtime();
     printf("Time started %ld", time_start);
     mynode = getfromBarrier();
     combining_barrier(&leaf[omp_get_thread_num()/d]);
     mysense = !mysense;
     time_finish = omp_get_wtime();
     thread_time = time_finish - time_start;
     avg_time[omp_get_thread_num()] += thread_time;
     total_time += thread_time;
         
    }
  
    }
     for(i = 0; i < N; i++) printf("Average Time taken by %d is %f\n", i, avg_time[i]/ITER);
     printf("Total time is %f \n", total_time);
    
    return 0;

}
