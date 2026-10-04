#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Exp.h"
#include "QatGenericFunctions/HermitePolynomial.h"
#include "QatGenericFunctions/GaussQuadratureRule.h"
#include "QatGenericFunctions/GaussIntegrator.h"
#include <Eigen/Dense>

using namespace Eigen;

// Print a matrix, setting roundoff-level entries (~1e-16) to zero for readability
void print(const std::string & title, MatrixXd M) {
  M = M.unaryExpr([](double v) { return std::abs(v) < 1e-12 ? 0.0 : v; });
  std::cout << title << std::endl
            << std::fixed << std::setprecision(4) << M << std::endl << std::endl;
}

int main (int argc, char **argv) {

  // Automatically generated:-------------------------:

  std::string usage= std::string("usage: ") + argv[0];
  if (argc!=1) {
    std::cout << usage << std::endl;
  }

  // -------------------------------------------------:

  const int nMax = 5;   // 5x5 submatrices: phi_0 ... phi_4

  // Points: phi_i * (A phi_j) = e^{-x^2} * poly of degree i+j+k, where k = 1 for x and D,
  // k = 2 for x^2, D^2 and H. Worst case 4+4+2 = 10, and 2N-1 >= 10  =>  N = 6.
  const int N = 6;

  Genfun::Variable X;
  Genfun::Exp exp;

  Genfun::GaussHermiteRule rule(N);
  Genfun::GaussIntegrator integrator(rule, Genfun::GaussIntegrator::INTEGRATE_DX);

  MatrixXd Xmat(nMax, nMax), X2mat(nMax, nMax);   // x, x^2
  MatrixXd Dmat(nMax, nMax), D2mat(nMax, nMax);   // D, D^2
  MatrixXd Hmat(nMax, nMax);                      // H

  // (A)_ij = <phi_i | A phi_j> = integral of phi_i * (A phi_j)
  for (int i = 0; i < nMax; i++)        // Row
  {
    Genfun::HermitePolynomial Hi(i, Genfun::TWIDDLE);
    Genfun::GENFUNCTION phi_i = exp(-X*X/2) * Hi;

    for (int j = 0; j < nMax; j++)      // Col
    {
      Genfun::HermitePolynomial Hj(j, Genfun::TWIDDLE);
      Genfun::GENFUNCTION phi_j = exp(-X*X/2) * Hj;

      Xmat(i, j)  = integrator(phi_i * (X * phi_j));                  // x phi_j
      X2mat(i, j) = integrator(phi_i * (X * X * phi_j));              // x^2 phi_j
      Dmat(i, j)  = integrator(phi_i * phi_j.prime());                // phi_j'
      D2mat(i, j) = integrator(phi_i * phi_j.prime().prime());        // phi_j''
      Hmat(i, j)  = -0.5 * D2mat(i, j) + 0.5 * X2mat(i, j);           // H = -1/2 D^2 + 1/2 x^2
    }
  }

  // a) position operator x
  print("(a) x:", Xmat);

  // b) x^2, compared with (x matrix)^2
  print("(b) x^2:", X2mat);
  print("(b) (x matrix)^2:", Xmat * Xmat);
  print("(b) x^2 - (x matrix)^2:", X2mat - Xmat * Xmat);

  // c) derivative D, and D^2 compared with (D matrix)^2
  print("(c) D:", Dmat);
  print("(c) D^2:", D2mat);
  print("(c) (D matrix)^2:", Dmat * Dmat);
  print("(c) D^2 - (D matrix)^2:", D2mat - Dmat * Dmat);

  // d) Hamiltonian
  print("(d) H:", Hmat);

  return 0;

}
