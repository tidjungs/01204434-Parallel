#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <mpi.h>

int num_clusters;
int row, col, iter;
int rank, size, num_per_proc;

float **input_data;
float **data;
float **recv_data;
int *centroidPositions;
float **centroids;


void print_array_2d(float **arr, int r, int c) {
  for (int i=0; i<r; i++) {
    for (int j=0; j<c; j++) {
      printf("%f ", arr[i][j]);
    }
    printf("\n");
  }
}

float cal_euclidean_distance(float *p1, float *p2) {
  float sum_square = 0;
  for (int i=0; i<col; i++) {
    sum_square += pow(p1[i] - p2[i], 2);
  }
  return sqrt(sum_square);
}

void random_centroids() {
  centroidPositions  = (int*)malloc(num_clusters * sizeof(int));
  for (int i=0; i<num_clusters; i++) {
    centroidPositions[i] = -1;
  }
  srand(time(NULL));
  int ct = 0;
  while (ct < num_clusters) {
    int r = rand() % row;
    int dup = 0;
    for (int i=0; i<=ct; i++) {
      if (r == centroidPositions[i]) {
        dup = 1;
        break;
      }
    }
    if (dup == 0) {
      centroidPositions[ct] = r;
      ct++;
    }
  }
  // for (int i=0; i<num_clusters; i++) {
  //   printf("%d\n", centroidPositions[i]);
  // }
  for (int i=0; i<num_clusters; i++) {
    for (int j=0; j<col; j++) {
      centroids[i][j] = input_data[j][centroidPositions[i]];
    }
  }
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  row = atoi(argv[2]);
  col = atoi(argv[3]);
  num_clusters = atoi(argv[4]);
  iter = atoi(argv[5]);
  num_per_proc = row/size;
  input_data = (float**)malloc(col * sizeof(float*));

  if (rank == 0) {
    char filename[50];
    sprintf(filename, "./%s", argv[1]);

    for (int i=0; i<col; i++)
      input_data[i] = (float*)malloc(row * sizeof(float));

    char line[1024];
    int i = 0;
    FILE* stream = fopen(filename, "r");
    while(fgets(line, 1024, stream))
    {
      char *pch;
      pch = strtok(line,",");
      int j = 0;
      while(pch != NULL)
      {
        input_data[j][i] = atoi(pch);
        pch = strtok(NULL, ",");
        j++;
      }
      i++;
    }
    // print_array_2d(input_data, col, row);
  }
  recv_data = (float**)malloc(col * sizeof(float*));
  for (int i=0; i<col; i++)
    recv_data[i] = (float*)malloc(num_per_proc * sizeof(float));

  data = (float**)malloc(num_per_proc * sizeof(float*));
  for (int i=0; i<num_per_proc; i++)
    data[i] = (float*)malloc(col * sizeof(float));

  for (int i=0; i<col; i++) {
    MPI_Scatter(input_data[i], num_per_proc, MPI_FLOAT, recv_data[i], num_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);
  }
  for(int i=0; i<num_per_proc; i++) {
    for (int j=0; j<col; j++) {
      data[i][j] = recv_data[j][i];
    }
  }

  // printf("rank %d\n", rank);
  // print_array_2d(data, num_per_proc, col);

  centroids = (float**)malloc(num_clusters * sizeof(float*));
  for (int i=0; i<num_clusters; i++)
    centroids[i] = (float*)malloc(col * sizeof(float));

  if (rank == 0) {
    //  random centroids with unique position
    random_centroids();
  }

  for (int i=0; i<num_clusters; i++) {
    MPI_Bcast(centroids[i], col, MPI_FLOAT, 0, MPI_COMM_WORLD);
  }
  printf("%d\n", rank);
  print_array_2d(centroids, num_clusters, col);
  // // create group array
  // int *group = (int*)malloc(row * sizeof(int));

  // // kmeans
  // while(iter > 0) {
  //   for (int i=0; i<row; i++) {
  //     float distance = INT_MAX;
  //     int newGroup = -1;
  //     for (int j=0; j<num_clusters; j++) {
  //       float newDistance = cal_euclidean_distance(data[i], centroids[j]);
  //       if (newDistance < distance) {
  //         distance = newDistance;
  //         newGroup = j;
  //       }
  //     }
  //     group[i] = newGroup;
  //   }
  //   // for (int i=0; i<row; i++) {
  //   //   printf("%d\n", group[i]);
  //   // }

  //   // calculate new centroid
  //   for (int i=0; i<num_clusters; i++) {
  //     for (int j=0; j<col; j++) {
  //       centroids[i][j] = 0;
  //     }
  //   }
  //   for (int i=0; i<num_clusters; i++) {
  //     int count = 0;
  //     for (int j=0; j<row; j++) {
  //       if (group[j] == i) {
  //         for (int k=0; k<col; k++) {
  //           centroids[i][k] += data[j][k];
  //         }
  //         count++;
  //       }
  //     }
  //     // printf("count %d\n", count);
  //     for (int k=0; k<col; k++) {
  //       centroids[i][k] /= count;
  //     }
  //   }
  //   iter--;
  // }
  // printCentroids(centroids);

  // // printf("%f\n", cal_euclidean_distance(data[0], data[1], col));
  // // for (int i=0; i<row; i++) {

  // // }

  free(data);
  free(input_data);
  free(recv_data);
  free(centroidPositions);
  free(centroids);
  // free(group);
  MPI_Finalize();
  return 0;
}