#include<mpi.h>
#include<sys/time.h>
#include<stdio.h>
#include "mcs_tree_barrier.h"

int main() {

	int rc = MPI_Init(NULL, NULL);
        if(rc!= MPI_SUCCESS) {
                printf("Error starting MPI program. Terminating\n");
                MPI_Abort(MPI_COMM_WORLD, rc);
        }
	struct timeval abc1, abc2;

	gettimeofday(&abc1, NULL);
	//printf("%lu\t%lu\n", abc1.tv_sec, abc1.tv_usec);
	int i;
	for(i=0;i<100000;i++) {
		mcs_barrier();
	}
	gettimeofday(&abc2, NULL);
	
	//printf("%lu\t%lu\n", abc2.tv_sec, abc2.tv_usec);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(rank == 0) {	
		long secdiff = abc2.tv_sec - abc1.tv_sec;
		long usecdiff = abc2.tv_usec - abc1.tv_usec;
		long totalusec=0;
		if(secdiff>=1) {
			long sectemp=abc1.tv_sec;
			long usectemp=abc1.tv_usec;
			while(sectemp<abc2.tv_sec) {
				totalusec += 1000000 - usectemp;
				usectemp=0;
				sectemp++;
			}
			totalusec += abc2.tv_usec;
		}
		else {
			totalusec = abc2.tv_usec - abc1.tv_usec;
		}
		printf("total time is %lu\n", totalusec);
	}
	MPI_Finalize();
	return 0;
}
