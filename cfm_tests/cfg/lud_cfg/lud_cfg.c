#define BLOCK_SIZE 16
#include <stdio.h>
void foo() {

  float dia[BLOCK_SIZE][BLOCK_SIZE];
  float peri_row[BLOCK_SIZE][BLOCK_SIZE];
  float peri_col[BLOCK_SIZE][BLOCK_SIZE];
  for (int a = 0; a < BLOCK_SIZE; a++) {
    for (int b = 0; b < BLOCK_SIZE; b++) {
      float val = a/(float)BLOCK_SIZE + b/(float)BLOCK_SIZE;
      dia[a][b] = val;
      peri_col[a][b] = val;
      peri_row[a][b] = val;
    }
  }

  int tid = 17, idx, i, j;

  if (tid < BLOCK_SIZE) { // peri-row
    idx = tid;
    for (i = 1; i < BLOCK_SIZE; i++) {
      for (j = 0; j < i; j++)
        peri_row[i][idx] -= dia[i][j] * peri_row[j][idx];
    }
  } else { // peri-col
    idx = tid - BLOCK_SIZE;
    for (i = 0; i < BLOCK_SIZE; i++) {
      for (j = 0; j < i; j++)
        peri_col[idx][i] -= peri_col[idx][j] * dia[j][i];
      peri_col[idx][i] /= dia[i][i];
    }
  }
  for (int a = 0; a < BLOCK_SIZE; a++) {
    for (int b = 0; b < BLOCK_SIZE; b++) {
      printf("%f %f\n", peri_row[a][b], peri_col[a][b]);
    }
  }
}

int main() { foo(); }