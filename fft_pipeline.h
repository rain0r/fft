#include "fft.h"
#include <cstdlib>
#include <iostream>

#ifndef C_FFT_PIPELINE_H
#define C_FFT_PIPELINE_H

class Matrix {
private:
	int n;
	int m;
	complex_t** values;

public:
	Matrix(int n_, int m_) {
		this->n = n_;
		this->m = m_;

		this->values = (complex_t **) malloc(sizeof(complex_t) * m);

		for (int i = 0; i <= m; i++) {
			this->values[m] = (complex_t *) malloc(sizeof(complex_t) * n);
		}
	}

	~Matrix() {
	}

	void init() {
		for (int i = 0; i <= this->m; i++) {
			for (int j = 0; j <= this->n; j++) {
				this->values[m][n].real = (float) i * j;
				this->values[m][n].imag = (float) i * j;
			}
		}
	}

	int getM() const {
		return m;
	}

	int getN() const {
		return n;
	}

	complex_t** getValues() const {
		return values;
	}
};

#endif
