#include "fft.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <queue>
#include <boost/thread.hpp>
#include <boost/timer/timer.hpp>
#include <immintrin.h>

#ifndef C_FFT_PIPELINE_H
#define C_FFT_PIPELINE_H

class Matrix {
 private:
  int m;
  int n;
  complex_t** values;

  // initialize with values
  void init() {
    for (int i = 0; i <= m; i++) {
      values[i] = new complex_t[n];
    }

    for (int i = 0; i <= this->m; i++) {
      for (int j = 0; j <= this->n; j++) {
        this->values[i][j].real = (static_cast<float>(rand())
            / static_cast<float>(RAND_MAX)) * 1000;
        this->values[i][j].imag = (static_cast<float>(rand())
            / static_cast<float>(RAND_MAX)) * 1000;
      }
    }
  }

 public:
  Matrix(int m, int n) {
    this->m = m;
    this->n = n;
    this->values = new complex_t*[m];
    this->init();
  }

  ~Matrix() {
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

  void print() {
    for (int i = 0; i <= this->m; i++) {
      for (int j = 0; j <= this->n; j++) {
        std::cout << "real: " << this->values[i][j].real << "\t imag: "
                  << this->values[i][j].imag << std::endl;
      }
    }
  }
};

class Trans {
 public:
  Trans() {
    this->locked = false;
  }

  bool isLocked() const {
    return locked;
  }

  void setLocked(bool locked) {
    this->locked = locked;
  }

 private:
  bool locked;

};

#endif
