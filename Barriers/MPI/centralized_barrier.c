#include<stdio.h>
#include<mpi.h>
#include "centralized_barrier.h"


int centralized_barrier(int *sense) {
	int numtasks, rank, len, rc;
	char hostname[MPI_MAX_PROCESSOR_NAME];
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(hostname, &len);
	//printf ("Number of tasks= %d My rank= %d Running on %s\n", numtasks,rank,hostname);

	int i;
	int msg = 1;
	int msg1;
	int tag_arrive = 0;
	int tag_release = 1;
	int count = numtasks-1;
	int Master = 0;
	MPI_Status status;
	if(rank != Master) {
		/*
		Inform the Master of my arrival
		*/
		MPI_Send(&msg, 1, MPI_INT, Master, tag_arrive, MPI_COMM_WORLD);

		/*
		Wait until Master releases me
		*/
		MPI_Recv(&msg1, 1, MPI_INT, Master, tag_release, MPI_COMM_WORLD, &status);
		*sense = msg1;
	}
	else {
		/*
		Count all arrivals
		*/
		for(i =1;i<numtasks;i++) {
			/* receive 1 message from all non-master processes */
			MPI_Recv(&msg, 1, MPI_INT, i, tag_arrive, MPI_COMM_WORLD, &status);
		}

		/*
		Now reverse the sense
		*/
		if(*sense == 0) {
			*sense = 1;
		}
		else {
			*sense = 0;
		}
		msg1 = *sense;
	
		/*
		and signal tasks to be released
		*/
		for(i=1;i<numtasks;i++) {
			rc = MPI_Send(&msg1, 1, MPI_INT, i, tag_release, MPI_COMM_WORLD);
		}
		if(rc != MPI_SUCCESS) {
			printf("Error in broadcast from Master\n");
		}
	}
	return 0;
}
