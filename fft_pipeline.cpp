#include "fft_pipeline.h"

int main(int argc, char* argv[]) {
  std::cout << "Blatt02/Aufgabe 2" << std::endl;

  int layer = 2;
  int n = 2;
  int m = 2;
  std::vector<Matrix> a, A, B, C;

  // Step One: Create a and apply FFT
  for (int i = 0; i <= layer; i++) {
    // Iterate over the layer
    Matrix *a_mat = new Matrix(n, m);
    FFT2D_inplace(a_mat->getValues(), a_mat->getM(), a_mat->getN(), 1);

    // Save Matrix in Vector
    a.push_back(*a_mat);
  }

  A = a;

  // Step Two: Create B
  for (int i = 0; i <= layer; i++) {
    // Iterate over the layers
    Matrix *B_mat = new Matrix(n, m);

    // Save Matrix in Vector
    B.push_back(*B_mat);
  }

  // Step Three: Multiply A and B
  // Iterate over the layer
  for (int i = 0; i <= layer; i++) {
    Matrix *C_mat = new Matrix(n, m);

    // Iterate over m and n
    for (int i = 0; i <= m; i++) {
      for (int j = 0; j <= n; j++) {

        // Multiply the values
        C_mat->getValues()[i][j].imag = A.at(i).getValues()[i][j].imag
            * B.at(i).getValues()[i][j].imag;

        C_mat->getValues()[i][j].real = A.at(i).getValues()[i][j].real
            * B.at(i).getValues()[i][j].real;

        C.push_back(*C_mat);
      }
    }
  }

  // Step Four: Sum up to one Matrix

  // Matrix to store the result
  Matrix *D_mat = new Matrix(n, m);

  // Iterate over the layer
  for (int i = 0; i <= layer; i++) {

    // Iterate over m and n
    for (int i = 0; i <= m; i++) {
      for (int j = 0; j <= n; j++) {

        // Sump up
        D_mat->getValues()[i][j].imag = C.at(i).getValues()[i][j].imag
            + C.at(i).getValues()[i][j].imag;

        D_mat->getValues()[i][j].real = C.at(i).getValues()[i][j].real
            + C.at(i).getValues()[i][j].real;
      }
    }
  }

  Matrix *d_mat = new Matrix(n, m);
  FFT2D_inplace(d_mat->getValues(), d_mat->getM(), d_mat->getN(), -1);

  return 0;
}

