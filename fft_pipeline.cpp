#include "fft_pipeline.h"

int main(int argc, char* argv[]) {
	std::cout << "Blatt02/Aufgabe 2" << std::endl;

	Matrix *mat = new Matrix(256, 256);
	mat->init();

	FFT2D_inplace(mat->getValues(), mat->getM(), mat->getN(), 1);

	return 0;
}

