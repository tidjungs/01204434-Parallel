#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <limits.h>

int num_clusters;
int row, col, iter;

void printCentroids(float **centroids) {
  for (int i=0; i<num_clusters; i++) {
    for (int j=0; j<col; j++) {
      printf("%f ", centroids[i][j]);
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

int main(int argc, char *argv[]) {
  char filename[50];
  sprintf(filename, "./%s", argv[1]);

  row = atoi(argv[2]);
  col = atoi(argv[3]);
  num_clusters = atoi(argv[4]);
  iter = atoi(argv[5]);

  // reading csv to array
  float **data = (float**)malloc(row * sizeof(float*));
  for (int i=0; i<row; i++)
    data[i] = (float*)malloc(col * sizeof(float));

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
      data[i][j] = atoi(pch);
      pch = strtok(NULL, ",");
      j++;
    }
    i++;
  }
  // for (int i = 0; i<row; i++) {
  //   for (int j = 0; j<col; j++) {
  //     printf("%f ", data[i][j]);
  //   }
  //   printf("\n");
  // }

    // random centroids with unique position
  int *centroidPositions  = (int*)malloc(num_clusters * sizeof(int));
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
  // for (int i=0; i<k; i++) {
  //   printf("%d\n", centroidPositions[i]);
  // }

  float **centroids = (float**)malloc(num_clusters * sizeof(float*));
  for (int i=0; i<num_clusters; i++)
    centroids[i] = (float*)malloc(col * sizeof(float));

  for (int i=0; i<num_clusters; i++) {
    for (int j=0; j<col; j++) {
      centroids[i][j] = data[centroidPositions[i]][j];
    }
  }
  // printCentroids(centroids);

  // create group array
  int *group = (int*)malloc(row * sizeof(int));

  // kmeans
  while(iter > 0) {
    for (int i=0; i<row; i++) {
      float distance = INT_MAX;
      int newGroup = -1;
      for (int j=0; j<num_clusters; j++) {
        float newDistance = cal_euclidean_distance(data[i], centroids[j]);
        if (newDistance < distance) {
          distance = newDistance;
          newGroup = j;
        }
      }
      group[i] = newGroup;
    }
    // for (int i=0; i<row; i++) {
    //   printf("%d\n", group[i]);
    // }

    // calculate new centroid
    for (int i=0; i<num_clusters; i++) {
      for (int j=0; j<col; j++) {
        centroids[i][j] = 0;
      }
    }
    for (int i=0; i<num_clusters; i++) {
      int count = 0;
      for (int j=0; j<row; j++) {
        if (group[j] == i) {
          for (int k=0; k<col; k++) {
            centroids[i][k] += data[j][k];
          }
          count++;
        }
      }
      // printf("count %d\n", count);
      for (int k=0; k<col; k++) {
        centroids[i][k] /= count;
      }
    }
    iter--;
  }
  printCentroids(centroids);

  // printf("%f\n", cal_euclidean_distance(data[0], data[1], col));
  // for (int i=0; i<row; i++) {

  // }

  free(data);
  free(centroidPositions);
  free(centroids);
  free(group);
  return 0;
}