#include "fft_pipeline.h"
#include "fft.h"
#include <cstdlib>
#include <iostream>

void init_input(complex_t **c, int nx, int ny) {
  int i, j;
  for (j = 0; j < 1; j++) {
    for (i = 0; i < 1; i++) {
      c[i][j].real = 1.3;
      c[i][j].imag = 1.1;
    }
  }
}

int main(int argc, char* argv[]) {
  std::cout << "Blatt02/Aufgabe 2" << std::endl;
  int layer_count = 4;
  int x = 256;  // row
  int y = 256;  // col

  complex_t** mat = (complex_t **) malloc(sizeof(complex_t) * x * y);
//  for (int i = 0; i < 5; ++i)
//    mat[i][i].real = i;

  init_input(mat, x, y);

  float f = (float) (rand()/10000);

  std::cout << "f: " << f << std::endl;
  std::cout << "real: " << mat[0][0].real << std::endl;

  return 0;
}

