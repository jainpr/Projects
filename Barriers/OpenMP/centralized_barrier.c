#include <omp.h>
#include <stdbool.h>

bool globalsense = true, localsense = true;

int count = 8;
#define N 8
#define ITER 2000
#pragma omp threadprivate(localsense)

int main(int argc, char *arv[])
{
     
    #pragma omp parallel num_threads(count) shared(globalsense, count)
    {
         
         localsense = !localsense;
    #pragma omp atomic     
         count --;
   
    if(count == 0) 
    {
         count = N;
         globalsense = localsense;    
    }      
    else
    
         while(globalsense != localsense);     
     
    } 
   
     
    return 0;
}
