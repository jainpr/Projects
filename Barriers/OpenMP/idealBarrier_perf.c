#include <omp.h>
#include <stdbool.h>

double time_start, time_finish, threadtime;
int count = 8;
#define N 8
#define ITER 500
//#pragma omp threadprivate(localsense)

#pragma omp threadprivate(time_start, time_finish, threadtime)
int main(int argc, char *arv[])
{
    double total_time, aveg_time[count];
    int i;
    for(i = 0; i < count; i++) aveg_time[i] = 0;

    for(i = 0; i < ITER; i++){
  
    #pragma omp parallel num_threads(count) shared(count)
    {
         time_start = omp_get_wtime();
         #pragma omp barrier 
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
