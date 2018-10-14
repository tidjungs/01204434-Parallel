#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {
  char filename[50];
  sprintf(filename, "./%s", argv[1]);

  int row = atoi(argv[2]);
  int col = atoi(argv[3]);
  int k = atoi(argv[4]);

  // reading csv to array
  int **data = (int **)malloc(row * sizeof(int *));
  for (int i=0; i<row; i++)
    data[i] = (int *)malloc(col * sizeof(int));

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
  for (int i = 0; i<row; i++) {
    for (int j = 0; j<col; j++) {
      printf("%d ", data[i][j]);
    }
    printf("\n");
  }

    // random k centroids with unique position
  int *centroidPositions  = (int *)malloc(k * sizeof(int *));
  for (int i=0; i<k; i++) {
    centroidPositions[i] = -1;
  }
  srand(time(NULL));
  int ct = 0;
  while (ct < k) {
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
  for (int i=0; i<k; i++) {
    printf("%d\n", centroidPositions[i]);
  }

  float **centroids = (float **)malloc(k * sizeof(float *));
  for (int i=0; i<k; i++)
    centroids[i] = (float *)malloc(col * sizeof(float));

  for (int i=0; i<k; i++) {
    for (int j=0; j<col; j++) {
      centroids[i][j] = data[centroidPositions[i]][j];
    }
  }

 for (int i=0; i<k; i++) {
    for (int j=0; j<col; j++) {
      printf("%f ", centroids[i][j]);
    }
    printf("\n");
  }
  free(data);
  free(centroidPositions);
  free(centroids);
  return 0;
}