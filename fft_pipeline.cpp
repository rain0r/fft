#include "fft_pipeline.h"

#define RETRIES 1000

typedef std::vector<Matrix> matrix_vector;
typedef std::queue<matrix_vector> vector_queue;
typedef std::queue<Matrix> matrix_queue;

//vec_mat a, A, B, C;

vector_queue A_work_queue = vector_queue();
vector_queue B_work_queue = vector_queue();
vector_queue C_work_queue = vector_queue();
matrix_queue D_work_queue = matrix_queue();

Trans A_mtx;
Trans B_mtx;
Trans C_mtx;
Trans D_mtx;

int layer = 16;
int n = 1024;
int m = 1024;

void a_to_A();
void create_B();
void AB_to_C();
void C_to_D();
void D_to_d();
bool finish();

// Step One: Create a and apply FFT
// a: Matrix(m, n)
void a_to_A() {
  std::cout << "a_to_A()" << std::endl;

  matrix_vector a;
  for (int i = 0; i <= layer; i++) {
    // Iterate over the layer
    Matrix *a_mat = new Matrix(m, n);
    FFT2D_inplace(a_mat->getValues(), a_mat->getM(), a_mat->getN(), 1);

    // Save Matrix in Vector
    a.push_back(*a_mat);
  }

  for (int i = 0; i < RETRIES; i++) {
    unsigned int status;
    if ((status = _xbegin()) == _XBEGIN_STARTED) {
      if (!A_mtx.isLocked()) {
        A_mtx.setLocked(true);

        // Actual work
        A_work_queue.push(a);
        // Actual work

        A_mtx.setLocked(false);
      }
      _xabort(0xff); /* lock busy */
    } else {
      // fallback with mutex

      A_mtx.mtxlock();

      // Actual work
      A_work_queue.push(a);
      // Actual work

      A_mtx.mtxunlock();
    }
    if ((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
      // wait
    } else if (!(status & _XABORT_RETRY))
      break;
  }

}

void create_B() {
  std::cout << "create_B()" << std::endl;

// Step Two: Create B
  matrix_vector B;
  for (int i = 0; i <= layer; i++) {
    // Iterate over the layers
    Matrix *B_mat = new Matrix(m, n);

    // Save Matrix in Vector
    B.push_back(*B_mat);
  }

  for (int i = 0; i < RETRIES; i++) {
    unsigned int status;
    if ((status = _xbegin()) == _XBEGIN_STARTED) {
      if (!B_mtx.isLocked()) {
        B_mtx.setLocked(true);

        // Actual work
        B_work_queue.push(B);
        // Actual work

        B_mtx.setLocked(false);
      }
      _xabort(0xff); /* lock busy */
    } else {
      // fallback with mutex

      B_mtx.mtxlock();

      // Actual work
      B_work_queue.push(B);
      // Actual work

      B_mtx.mtxunlock();
    }
    if ((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
      // wait
    } else if (!(status & _XABORT_RETRY))
      break;
  }
}
void AB_to_C() {
  std::cout << "AB_to_C()" << std::endl;

  matrix_vector C;
  while (!finish()) {

    if (!A_work_queue.empty() && !B_work_queue.empty()) {
      matrix_vector A;
      matrix_vector B;

      for (int i = 0; i < RETRIES; i++) {
        unsigned int status;
        if ((status = _xbegin()) == _XBEGIN_STARTED) {
          if (!A_mtx.isLocked()) {
            A_mtx.setLocked(true);

            // Actual work
            matrix_vector A = A_work_queue.front();
            A_work_queue.pop();
            // Actual work

            A_mtx.setLocked(false);
          }
          _xabort(0xff); /* lock busy */
        } else {
          // fallback with mutex
          A_mtx.mtxlock();

          // Actual work
          matrix_vector A = A_work_queue.front();
          A_work_queue.pop();
          // Actual work

          A_mtx.mtxunlock();
        }
        if ((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
          // wait
        } else if (!(status & _XABORT_RETRY))
          break;
      }

      for (int i = 0; i < RETRIES; i++) {
        unsigned int status;
        if ((status = _xbegin()) == _XBEGIN_STARTED) {
          if (!B_mtx.isLocked()) {
            B_mtx.setLocked(true);

            // Actual work
            matrix_vector B = B_work_queue.front();
            B_work_queue.pop();
            // Actual work

            B_mtx.setLocked(false);
          }
          _xabort(0xff); /* lock busy */
        } else {
          // fallback with mutex
          B_mtx.mtxlock();

          // Actual work
          matrix_vector B = B_work_queue.front();
          B_work_queue.pop();
          // Actual work

          B_mtx.mtxunlock();
        }
        if ((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
          // wait
        } else if (!(status & _XABORT_RETRY))
          break;
      }

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

      // Push C on the worker queue
      for (int i = 0; i < RETRIES; i++) {
        unsigned int status;
        if ((status = _xbegin()) == _XBEGIN_STARTED) {
          if (!C_mtx.isLocked()) {
            C_mtx.setLocked(true);

            // Actual work
            C_work_queue.push(C);
            // Actual work

            C_mtx.setLocked(false);
          }
          _xabort(0xff); /* lock busy */
        } else {
          // fallback with mutex
          C_mtx.mtxlock();

          // Actual work
          C_work_queue.push(C);
          // Actual work

          C_mtx.mtxunlock();
        }
        if ((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
          // wait
        } else if (!(status & _XABORT_RETRY))
          break;
      }
    }
  }
}

// Step Four: Sum up to one Matrix
void C_to_D() {
  std::cout << "C_to_D()" << std::endl;

  while (!finish()) {

    if (!C_work_queue.empty()) {

      matrix_vector C;

      for (int i = 0; i < RETRIES; i++) {
        unsigned int status;
        if ((status = _xbegin()) == _XBEGIN_STARTED) {
          if (!C_mtx.isLocked()) {
            C_mtx.setLocked(true);

            // Actual work
            matrix_vector C = C_work_queue.front();
            C_work_queue.pop();
            // Actual work

            C_mtx.setLocked(false);
          }
          _xabort(0xff); /* lock busy */
        } else {
          // fallback with mutex
          C_mtx.mtxlock();

          // Actual work
          matrix_vector C = C_work_queue.front();
          C_work_queue.pop();
          // Actual work

          C_mtx.mtxunlock();
        }
        if ((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
          // wait
        } else if (!(status & _XABORT_RETRY))
          break;
      }

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

      for (int i = 0; i < RETRIES; i++) {
        unsigned int status;
        if ((status = _xbegin()) == _XBEGIN_STARTED) {
          if (!D_mtx.isLocked()) {
            D_mtx.setLocked(true);

            // Actual work
            D_work_queue.push(*D_mat);
            // Actual work

            D_mtx.setLocked(false);
          }
          _xabort(0xff); /* lock busy */
        } else {
          // fallback with mutex
          D_mtx.mtxlock();

          // Actual work
          D_work_queue.push(*D_mat);
          // Actual work

          D_mtx.mtxunlock();
        }
        if ((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
          // wait
        } else if (!(status & _XABORT_RETRY))
          break;
      }

    }
  }
}

// Step Five: apply IFFT
void D_to_d() {
  std::cout << "D_to_d()" << std::endl;

  while (!finish()) {
    if (!D_work_queue.empty()) {

      for (int i = 0; i < RETRIES; i++) {
        unsigned int status;
        if ((status = _xbegin()) == _XBEGIN_STARTED) {
          if (!D_mtx.isLocked()) {
            D_mtx.setLocked(true);

            // Actual work
            Matrix D = D_work_queue.front();
            D_work_queue.pop();
            // Actual work

            D_mtx.setLocked(false);
          }
          _xabort(0xff); /* lock busy */
        } else {
          // fallback with mutex
          D_mtx.mtxlock();

          // Actual work
          Matrix D = D_work_queue.front();
          D_work_queue.pop();
          // Actual work

          D_mtx.mtxunlock();
        }
        if ((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
          // wait
        } else if (!(status & _XABORT_RETRY))
          break;
      }

      Matrix *d_mat = new Matrix(m, n);
      FFT2D_inplace(d_mat->getValues(), d_mat->getM(), d_mat->getN(), -1);

      std::cout << "End" << std::endl;
    }
  }
}

bool finish() {
  int count = A_work_queue.size() + B_work_queue.size() + C_work_queue.size()
      + D_work_queue.size();

//  std::cout << "A: " << A_q.size() << std::endl;
//  std::cout << "B: " << B_q.size() << std::endl;
//  std::cout << "C: " << C_q.size() << std::endl;
//  std::cout << "D: " << D_q.size() << std::endl << std::endl;
//  std::cout << count << std::endl;

  if (count == 0)
    return true;
  else
    return false;
}

int main(int argc, char* argv[]) {
  std::cout << "Blatt02/Aufgabe 2" << std::endl;
  boost::timer::auto_cpu_timer timer;
  boost::thread_group fft_g;

  boost::thread *t1 = new boost::thread(a_to_A);
  fft_g.add_thread(t1);

  boost::thread *t2 = new boost::thread(create_B);
  fft_g.add_thread(t2);

  boost::thread *t3 = new boost::thread(AB_to_C);
  fft_g.add_thread(t3);

  boost::thread *t4 = new boost::thread(C_to_D);
  fft_g.add_thread(t4);

  boost::thread *t5 = new boost::thread(D_to_d);
  fft_g.add_thread(t5);

  fft_g.join_all();

  return 0;
}
