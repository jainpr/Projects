#include <omp.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

int n = 8;
int d = 2;

#define N 8
#define ITER 500

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

/*Directly Building a tree with 2 nodes/ parent*/
int main()
{
    init_Barrier();
    #pragma omp parallel num_threads(N) shared(leaf)
    {
  
     mynode = getfromBarrier();
     combining_barrier(&leaf[omp_get_thread_num()/d]);
     mysense = !mysense;
          
    }
  
     return 0;

}
