#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

unsigned long long m[2000][2000];
int buf[2000];
unsigned long long input[2000];
int n;

int main(int argc, char *argv[]) {

  char filename[50];
  sprintf(filename, "./%s", argv[1]);

  int count = atoi(argv[2]);
	FILE *file;
	file = fopen(filename, "r");
	fread(buf, 4, count, file);
	for (int i = 0; i < count; i++) {
    input[i] = buf[i];
	}
  n = count - 1;

  for (int len=1; len<n; len++) {
    for (int i=0; i<n-len; i++) {
      int j = i + len;
      m[i][j] = ULONG_MAX;
      for (int k=i; k < j; k++) {
        unsigned long long cost = m[i][k] + m[k+1][j] + input[i]*input[k+1]*input[j+1];
        if (cost < m[i][j]) {
           m[i][j] = cost;
        }
      }
    }
  }
  printf("ans: %llu\n",m[0][n-1]);
  return 0;
}