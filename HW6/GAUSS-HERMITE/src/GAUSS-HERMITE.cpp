#include <cstdlib>
#include <iostream>
#include <string>
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Exp.h"
#include "QatGenericFunctions/HermitePolynomial.h"
#include "QatGenericFunctions/GaussQuadratureRule.h"
#include "QatGenericFunctions/GaussIntegrator.h"
#include <Eigen/Dense>

using namespace Eigen;

int main (int argc, char * * argv) {

  // Automatically generated:-------------------------:

  std::string usage= std::string("usage: ") + argv[0];
  if (argc!=1) {
    std::cout << usage << std::endl;
  }


  const int nMax = 6;         // phi_0 ... phi_5
  const int N    = 6;         // Gauss-Hermite points (minimum: 2N-1 >= 10)

  MatrixXd mat(nMax, nMax);

  Genfun::Variable X;
  Genfun::Exp exp;

  // Integrator
  Genfun::GaussHermiteRule rule(N);   // N = number of points
  Genfun::GaussIntegrator integrator(rule, Genfun::GaussIntegrator::INTEGRATE_DX);

  for(int i = 0; i<nMax; i++)    // Row
  {
    Genfun::HermitePolynomial Hi(i, Genfun::TWIDDLE);
    Genfun::GENFUNCTION phi_i = exp(-X*X/2) * Hi;

    for(int j = 0; j<nMax; j++)  // Col
    {
      Genfun::HermitePolynomial Hj(j, Genfun::TWIDDLE);
      Genfun::GENFUNCTION phi_j = exp(-X*X/2) * Hj;

      mat(i, j) = integrator(phi_i * phi_j);   // <phi_i|phi_j>
    }
  }

  std::cout << "A (N = " << N << " points):" << std::endl
            << mat << std::endl;

  return 0;
}
