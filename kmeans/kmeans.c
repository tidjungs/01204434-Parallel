#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  char filename[50];
  sprintf(filename, "./%s", argv[1]);
  FILE* stream = fopen(filename, "r");

  int row = atoi(argv[2]);
  int col = atoi(argv[3]);

  int **data = (int **)malloc(row * sizeof(int *));
  for (int i=0; i<row; i++)
    data[i] = (int *)malloc(col * sizeof(int));

  char line[1024];
  int i = 0;
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
  free(data);
  return 0;
}