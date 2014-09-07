#include<mpi.h>
#include "mcs_tree_barrier.h"
#include<stdio.h>

int main(int *argc, char **argv) {

	int rc;
	rc = MPI_Init(NULL, NULL);
        if(rc!= MPI_SUCCESS) {
                printf("Error starting MPI program. Terminating\n");
                MPI_Abort(MPI_COMM_WORLD, rc);
        }
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	printf("before barrier 1, my rank is %d\n", rank);
	fflush(stdout);
	int i;
	for(i=0;i<rand();i++);
	mcs_barrier();

	printf("after barrier 1, my rank is %d\n", rank);
	fflush(stdout);

	for(i=0;i<rand();i++);
        mcs_barrier();

        printf("after barrier 2, my rank is %d\n", rank);
	fflush(stdout);
	for(i=0;i<rand();i++);
        mcs_barrier();

        printf("after barrier 3, my rank is %d\n", rank);
	fflush(stdout);
	for(i=0;i<rand();i++);
        mcs_barrier();

        printf("after barrier 4, my rank is %d\n", rank);
	fflush(stdout);

	MPI_Finalize();	
	return 0;
}
