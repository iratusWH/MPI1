#include <stdio.h>
#include <cstdlib>
#include "mpi.h"

#define N 32768
#define S1Const 1567.013123
#define size 2

int main(int argc, char** argv) {

	int rank;

	double startTime, endTime;

	float A[N], B[N], C[N], Y[N];
	int S1 = S1Const;

	MPI_Status stat;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	srand(12);

	printf("Rank: %d\n", rank);

	if (rank == 0) {

		// input random nums from 0 to 2000 
		for (int i = 0; i < N; i++) {
			A[i] = 0.531 * (rand() % 2001);
			B[i] = i * 250.123;
			C[i] = i * 150.321;
		}

		startTime = MPI_Wtime();
		for (int cycles = 0; cycles < 10000; cycles++) {
			for (int i = 1; i < size; i++) {
				MPI_Send(&A[(int)i * N / size], (int)N / size,
					MPI_FLOAT, i, 0, MPI_COMM_WORLD);
			}

			for (int i = 0; i < N / size; i++)
				Y[i] = A[i] * S1 + C[i] / (A[i] + B[i]);

			for (int i = 1; i < size; i++) {
				MPI_Recv(&Y[(int)N * i / size], (int)N / size,
					MPI_FLOAT, i, 0, MPI_COMM_WORLD, &stat);
			}
		}
		endTime = MPI_Wtime();

		// output
		/*
		printf("Y = [");
		for (int i = 0; i < N; i++)
			printf("%f ", Y[i]);
		printf("]\n");
		*/

		printf("TIME=%f\n", endTime - startTime);
	}
	if (rank != 0 && rank < size) {

		// computing
		// Yi = Ai * S1 + Ci/(Ai+Bi)
		for (int i = 0; i < N; i++) {
			B[i] = i * 250.123;
			C[i] = i * 150.321;
		}
		for (int cycles = 0; cycles < 10000; cycles++) {
			MPI_Recv(&A[(int)N * rank / size], (int)N / size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &stat);

			for (int i = N * rank / size; i < N * (rank + 1) / size; i++)
				Y[i] = A[i] * S1 + C[i] / (A[i] + B[i]);

			MPI_Send(&Y[(int)N * rank / size], (int)N / size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
}