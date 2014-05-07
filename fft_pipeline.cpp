#include "fft_pipeline.h"

std::vector<Matrix> a, A, B, C;
boost::thread_group proc_g;

int layer = 16;
int n = 1024;
int m = 1024;

void a_to_A();
void AB_to_C();
void create_B();
void C_to_D();
void D_to_d();

// Step One: Create a and apply FFT
// a: Matrix(m, n)
void a_to_A() {
//  std::cout << "a_to_A()" << std::endl;

  for (int i = 0; i <= layer; i++) {
    // Iterate over the layer
    Matrix *a_mat = new Matrix(m, n);
    FFT2D_inplace(a_mat->getValues(), a_mat->getM(), a_mat->getN(), 1);

    // Save Matrix in Vector
    a.push_back(*a_mat);
  }
  A = a;
  create_B();
}
void create_B() {
//  std::cout << "create_B()" << std::endl;

  // Step Two: Create B
  for (int i = 0; i <= layer; i++) {
    // Iterate over the layers
    Matrix *B_mat = new Matrix(m, n);

    // Save Matrix in Vector
    B.push_back(*B_mat);
  }
  AB_to_C();
}
void AB_to_C() {
//  std::cout << "AB_to_C()" << std::endl;

  // Step Three: Multiply A and B
  // Iterate over the layer
  for (int lay = 0; lay <= layer; lay++) {
    Matrix *C_mat = new Matrix(m, n);

    // Iterate over m and n
    for (int i = 0; i <= m; i++) {
      for (int j = 0; j <= n; j++) {

        // Multiply the values
        C_mat->getValues()[i][j].imag = A.at(lay).getValues()[i][j].imag
            * B.at(lay).getValues()[i][j].imag;

        C_mat->getValues()[i][j].real = A.at(lay).getValues()[i][j].real
            * B.at(lay).getValues()[i][j].real;

        C.push_back(*C_mat);
      }
    }
  }
  C_to_D();
}
void C_to_D() {
//  std::cout << "C_to_D()" << std::endl;

  // Step Four: Sum up to one Matrix

  // Matrix to store the result
  Matrix *D_mat = new Matrix(m, n);

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
  D_to_d();
}
void D_to_d() {
//  std::cout << "D_to_d()" << std::endl;

  // Step Five: apply IFFT
  Matrix *d_mat = new Matrix(m, n);
  FFT2D_inplace(d_mat->getValues(), d_mat->getM(), d_mat->getN(), -1);
}

int main(int argc, char* argv[]) {
  std::cout << "Blatt02/Aufgabe 2" << std::endl;
  boost::timer::auto_cpu_timer timer;

  a_to_A();

  return 0;
}

