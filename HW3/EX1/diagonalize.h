#ifndef DIAGONALIZE_H
#define DIAGONALIZE_H

#include <Eigen/Dense>
#include <complex>
#include <array>

using namespace Eigen;
using namespace std;

struct DiagonalResult {
    std::array<std::complex<double>, 3> eigenvalues;
    Matrix3cd eigenvectors;
};

DiagonalResult diagonalize(Matrix3cd A);

#endif
