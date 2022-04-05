#include <stdio.h>
#include <cstdlib>

#define N 8
#define S1Const 15

int main(int argc, char** argv) {

	int A[N];
	int B[N];
	int C[N];
	int Y[N];
	int S1 = S1Const;

	srand(12);

	// Ввод случайных цифр от 0 до 2000 
	for (int i = 0; i < N; i++) {
		A[i] = rand() % 2001;
		B[i] = rand() % 2001;
		C[i] = rand() % 2001;
	}
	
	// Yi = Ai * S1 + Ci/(Ai+Bi)
	for (int i = 0; i < N; i++)
		Y[i] = A[i] * S1 + C[i] / (A[i] + B[i]);

	// Вывод
	printf("Y = [");
	for (int i = 0; i < N; i++)
		printf("%d ", Y[i]);
	printf("]\n");
}