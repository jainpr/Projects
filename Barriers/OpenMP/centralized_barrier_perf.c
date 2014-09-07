#include <omp.h>
#include <stdbool.h>

bool globalsense = true, localsense = true;

int count = 6;
double time_start, time_finish, threadtime;
#define N 6
#define ITER 8000
#pragma omp threadprivate(localsense, time_start, time_finish, threadtime)

int main(int argc, char *arv[])
{
    int i;
    double total_time, aveg_time[count];
  
    for(i = 0; i < count; i++) aveg_time[i] = 0;

    for(i = 0; i < ITER; i++){
 
    #pragma omp parallel num_threads(count) shared(globalsense, count)
    {
         time_start = omp_get_wtime();
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
         time_finish = omp_get_wtime();
         threadtime = time_finish - time_start;
         total_time += threadtime;
         aveg_time[omp_get_thread_num()] += threadtime;
 
 
    } 
   }
    for(i = 0; i < count; i++) printf("Average time taken by thread %d is %f\n", i, aveg_time[i]/ITER);
    printf("Total Time taken by the Threads %f \n", total_time);
    
   
    return 0;
}
