#include<stdio.h>
#include "mcs_tree_barrier.h"
#include<stdlib.h>
#include<math.h>

void mcs_barrier() {
	
        int numtasks, rank, len, rc;
        char hostname[MPI_MAX_PROCESSOR_NAME];
        MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Get_processor_name(hostname, &len);
	MPI_Status status;
	int HC[4];
	int CN[4];

	/*
	Initialize the two data structures for arrival tree(4-ary tree)
	*/
	// l denotes number of children
	int l = numtasks - 4*rank-1 ;
	if(l<=0) {
		l=0;
	} else if(l>=4) {
		l=4;
	}
	int i;
	int num_children = 0;
	for(i=0;i<4;i++){
		if(i<l) {
			num_children++;
			HC[i] = 1;
			CN[i] = 0;
		}
		else {
			HC[i] = 0;
			CN[i] = -1;
		}
	}

//	printf("Rank is %d, children in arrival tree are %d\n", rank, num_children);
	/*
	Initialize the data structure for wakeup tree(binary tree)
	*/
	
	int CP[2];
	int num_wakeup_children = 0;
	l = numtasks - 2*rank-1;
	if(l<=0) {
		l=0;
	} else if(l>=2) {
		l=2;
	}

	for(i=0; i<2;i++) {
		if(i<l) {
			num_wakeup_children++;
			CP[i] = 0;
		}
		else {
			CP[i] = -1;
		}
	}	

//	printf("Rank is %d, children in wakeup tree are %d\n", rank, num_wakeup_children);
	//printf("data structures initialized\n");
	/*
	Tasks are numbered from 0 to numtasks-1. In MCS tree barrier, a parent i's children are -
	4(i+1)-0, 4(i+1)-1, 4(i+1)-2 and 4(i+1)-3, given that all these children are < numtasks
	
	For a child i, its parent is ceil(int(i/4))-1
	*/

	/*
	Arrival Procedure underway
	*/

	// Send message to parent, if all children have arrived

	// wait for all children to arrive

	// Receive messages from children;
	for(i=0 ;i <num_children; i++){
		MPI_Recv(&CN[i], 1, MPI_INT, 4*(rank+1)-(3-i), rank, MPI_COMM_WORLD, &status);
	}
	int sum = 0;
	while(sum<num_children) {
		sum = 0;
		for(i=0;i<num_children;i++) {
			sum = sum+CN[i];
		}
	}

	fflush(stdout);
	//printf("flag1 for rank %d\n", rank);	
	// All children(if any) arrived, signal to parent

	int val = 1;
	float dst1 = (float)rank/4;
	int dst2;
	dst2 = ceil(dst1)-1;
	if(rank!=0) {

		MPI_Send(&val, 1, MPI_INT, dst2, dst2, MPI_COMM_WORLD);
	} 

	//printf("Arrivals completed\n");
	fflush(stdout);
	/*
	Begin Wakeup Process, starting from root node P0
	*/

	int received = 0;
	int key=1;
	int src = ceil((float)rank/2)-1;	
	if(rank!=0) {
		//printf("Rank %d waiting to receive from %d\n", rank, src);
		MPI_Recv(&received, 1, MPI_INT, src, src, MPI_COMM_WORLD, &status);
		while(received!=1);
	}

	for(i=0; i<num_wakeup_children; i++) {
		CP[i]=1;
		//printf("rank %d sending to %d \n", rank, 2*(rank+1)-(1-i));
		MPI_Send(&key, 1, MPI_INT, 2*(rank+1)-(1-i), rank, MPI_COMM_WORLD);
	}

	// Wakeup complete

	return;
}
