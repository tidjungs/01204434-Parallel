#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <mpi.h>

// 2 dimention-array for compute matrix chain
unsigned long long m[2001][2001];

// buffer from read input
int buf[2001];

unsigned long long input[2001];

int inputSize;
int size;
int rank;


void printMatrix() {
  for (int i = 0; i < inputSize - 1; i++) {
    for (int j = 0; j < inputSize - 1; j++) {
      printf("%llu ", m[i][j]);
    }
    printf("\n");
  }
  printf("=====================\n");
}

void readInputData(char fileName[]) {
  char pathToFile[50];
  sprintf(pathToFile, "./%s", fileName);
  FILE *file;
  file = fopen(pathToFile, "r");
  fread(buf, 4, inputSize, file);
  for (int i = 0; i < inputSize; i++) {
    input[i] = buf[i];
  }
}

void sendDataToSlave(int slaveCount) {
  for (int i = 1; i < slaveCount; i ++) {
    int startIndex = i*(inputSize-1)/size;
    int dataSize = inputSize - startIndex;
    MPI_Send(&input[startIndex], dataSize, MPI_UNSIGNED_LONG_LONG, i, 0, MPI_COMM_WORLD);
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
    int startIndex = rank*(inputSize-1)/size;
    int dataSize = inputSize - startIndex;
    MPI_Recv(&input[startIndex], dataSize, MPI_UNSIGNED_LONG_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  int r = 1;
  int n = inputSize - 1;
  int gap = n / size;

  for (int r=1; r<n; r++) {
    int startIndex = rank*(inputSize-1)/size;
    int endIndex = (rank+1)*(inputSize-1)/size;
    for (int i=startIndex; i<endIndex; i++) {
      int j = i+r;
      if (j < n) {
        m[i][j] = ULONG_MAX;
        for (int k=i; k<j; k++) {
          unsigned long long cost = m[i][k] + m[k+1][j] + input[i]*input[k+1]*input[j+1];
          // printf("%d %d : %llu from %d\n", i, j, cost, rank);
          if (cost < m[i][j]) {
            m[i][j] = cost;
          }
        }
      }

      if (rank == 0) {
        for (int k=1; k<=(size - rank - 1); k ++) {
          MPI_Recv(&m[i+gap*k][j+gap*k], 1, MPI_UNSIGNED_LONG_LONG, rank+k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
      } else if (rank == size - 1) {
        for (int k=1; k<=rank; k++) {
          MPI_Send(&m[i][j], 1, MPI_UNSIGNED_LONG_LONG, rank-k, 0, MPI_COMM_WORLD);
        }
      } else {
        for (int k=1; k<=(size - rank - 1); k ++) {
          MPI_Recv(&m[i+gap*k][j+gap*k], 1, MPI_UNSIGNED_LONG_LONG, rank+k, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        for (int k=1; k<=rank; k++) {
          MPI_Send(&m[i][j], 1, MPI_UNSIGNED_LONG_LONG, rank-k, 0, MPI_COMM_WORLD);
        }
      }
    }
  }

  if (rank == 0) {
    printf("ans: %llu\n",m[0][n-1]);
  }
  MPI_Finalize();
  return 0;
}