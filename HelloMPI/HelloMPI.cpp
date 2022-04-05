#include <stdio.h>
#include <cstdlib>
#include "mpi.h"

#define N 8
#define S1Const 15

int main(int argc, char** argv) {

	int rank, size;

	double startTime, endTime;

	int A[N], B[N], C[N], Y[N];
	int S1 = S1Const;

	MPI_Status stat;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	srand(12);

	if (rank == 0) {
		printf("Rank: %d\n", rank);
		// input random nums from 0 to 2000 
		for (int i = 0; i < N; i++) {
			A[i] = rand() % 2001;
			B[i] = rand() % 2001;
			C[i] = rand() % 2001;
		}

			startTime = MPI_Wtime();
			for (int i = 1; i < size; i++) {
				MPI_Send(&A[N * i / size], N / size, MPI_INTEGER, i, 0, MPI_COMM_WORLD);
				MPI_Send(&B[N * i / size], N / size, MPI_INTEGER, i, 1, MPI_COMM_WORLD);
				MPI_Send(&C[N * i / size], N / size, MPI_INTEGER, i, 2, MPI_COMM_WORLD);
			}

			for (int i = N * rank / size; i < (N * (rank + 1) / size) - 1; i++)
				Y[i] = A[i] * S1 + C[i] / (A[i] + B[i]);

			for (int i = 0; i < size; i++) {
				MPI_Recv(&Y[N * i / size], N / size, MPI_INTEGER, i, 0, MPI_COMM_WORLD, &stat);
			}
		endTime = MPI_Wtime();

		// output
		printf("Y = [");
		for (int i = 0; i < N; i++)
			printf("%d ", Y[i]);
		printf("]\n");


	}
	else {
		printf("Rank: %d\n", rank);
		//[N * rank / size, N * (rank + 1)/size - 1]
		// computing
		// Yi = Ai * S1 + Ci/(Ai+Bi)
			MPI_Recv(&A[N * rank / size], N / size, MPI_INTEGER, rank, 0, MPI_COMM_WORLD, &stat);
			MPI_Recv(&B[N * rank / size], N / size, MPI_INTEGER, rank, 1, MPI_COMM_WORLD, &stat);
			MPI_Recv(&C[N * rank / size], N / size, MPI_INTEGER, rank, 2, MPI_COMM_WORLD, &stat);

			for (int i = N * rank / size; i < (N * (rank + 1) / size) - 1; i++)
				Y[i] = A[i] * S1 + C[i] / (A[i] + B[i]);

			MPI_Send(&Y[N * rank / size], N / size, MPI_INTEGER, rank, 0, MPI_COMM_WORLD);
	
	}

	MPI_Finalize;
}