#include <iostream>
#include <complex>
#include <cmath>

int main() {
  std::cout << "Testing Complex Trigonometric/Hyperbolic Identities\n";
  std::cout << "\n";

  std::cout << "Identity 1 sin(ix) = i*sinh(x)\n";
  std::cout << "Identity 2 cos(ix) = cosh(x)\n\n";

  double x_values[] = {0.5, 1.0, 1.5, 2.0, 2.5};

  for (double x : x_values) {
    // Identity 1
    std::complex<double> ix(0.0, x);
    std::complex<double> sin_ix = std::sin(ix);
    std::complex<double> i_sinh_x(0.0, std::sinh(x));
    double error1 = std::abs(sin_ix - i_sinh_x);

    // Identity 2
    std::complex<double> cos_ix = std::cos(ix);
    double cosh_x = std::cosh(x);
    double error2 = std::abs(cos_ix - cosh_x);

    std::cout << "x = " << x << "\n";
    std::cout << "  sin(ix) = " << sin_ix << ", i*sinh(x) = " << i_sinh_x
              << ", error = " << error1 << "\n";
    std::cout << "  cos(ix) = " << cos_ix << ", cosh(x) = " << cosh_x
              << ", error = " << error2 << "\n\n";
  }

  return 0;
}
