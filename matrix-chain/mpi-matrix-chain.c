#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <mpi.h>

// 2 dimention-array for compute matrix chain
unsigned long long m[2000][2000];

// buffer from read input
int buf[2000];

unsigned long long input[2000];

int inputSize;
int size;
int rank;


// void printMatrix() {
//   for (int i = 0; i < n; i++) {
//     for (int j = 0; j < n; j++) {
//       printf("%llu ", m[i][j]);
//     }
//     printf("\n");
//   }
//   printf("=====================\n");
// }

void readInputData(char fileName[]) {
  char pathToFile[50];
  sprintf(pathToFile, "./%s", fileName);
  FILE *file;
  file = fopen(pathToFile, "r");
  fread(buf, 4, inputSize, file);
  for (int i = 0; i < inputSize; i++) {
    input[i] = buf[i];
    printf("%llu\n", input[i]);
  }
}

void sendDataToSlave(int slaveCount) {
  for (int i = 1; i < slaveCount; i ++) {
    int sendingSize = inputSize >> i;
    int startIndex = inputSize - sendingSize;
    MPI_Send(&input[startIndex], sendingSize, MPI_UNSIGNED_LONG_LONG	, i, 0, MPI_COMM_WORLD);
  }
}

int main(int argc, char *argv[]) {

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  inputSize = atoi(argv[2]);

  if (rank == 0) {
    readInputData(argv[1]);
    sendDataToSlave(size);
  } else {
    int receiveSize = inputSize >> rank;
    int startIndex = inputSize - receiveSize;
    MPI_Recv(&input[startIndex], receiveSize, MPI_UNSIGNED_LONG_LONG	, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for (int i=startIndex; i < startIndex + receiveSize; i++) {
      printf("print from %d : %llu\n", rank, input[i]);
    }
    // printf("%d\n", a);
  }
  // n = count - 1;

  // for (int len=1; len<n; len++) {
  //   for (int i=0; i<n-len; i++) {
  //     int j = i + len;
  //     m[i][j] = ULONG_MAX;
  //     for (int k=i; k < j; k++) {
  //       unsigned long long cost = m[i][k] + m[k+1][j] + input[i]*input[k+1]*input[j+1];
  //       if (cost < m[i][j]) {
  //         // printMatrix();
  //          m[i][j] = cost;
  //       }
  //     }
  //     // printf("===========\n");
  //   }
  // }
  // printf("ans: %llu\n",m[0][n-1]);
  MPI_Finalize();
  return 0;
}