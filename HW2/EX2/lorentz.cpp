#include <iostream>
#include <cmath>

int main() {
  std::cout << "Computing Lorentz Factor gamma\n";
  std::cout << "\n";

  // Test values
  double beta_values[] = {0.9, 0.99, 0.999, 0.9999};
  double epsilon_values[] = {0.1, 0.01, 0.001, 0.0001};

  std::cout << "Method 1: gamma = 1/sqrt(1 - beta^2)\n";
  std::cout << "Method 2: gamma = 1/sqrt((2 - eps)eps) where eps = 1 - beta\n\n";

  for (int i = 0; i < 4; ++i) {
    double beta = beta_values[i];
    double epsilon = epsilon_values[i];

    double gamma1 = 1.0 / std::sqrt(1.0 - beta * beta);
    double gamma2 = 1.0 / std::sqrt((2.0 - epsilon) * epsilon);
    double fractional_error = std::abs(gamma1 - gamma2) / gamma1;

    std::cout << "beta = " << beta << "\n";
    std::cout << "  gamma (Method 1) = " << gamma1 << "\n";
    std::cout << "  gamma (Method 2) = " << gamma2 << "\n";
    std::cout << "  Fractional Error = " << fractional_error << "\n\n";
  }

  std::cout << "\nFinding maximum beta with fractional error <= 0.001...\n";

  double eps_low = 0;
  double eps_high = 1/3.0;
  double target_error = 0.001;

  while ((eps_high-eps_low)/eps_high > 0.0001) {
    double eps_mid = (eps_low + eps_high) / 2.0;
    double beta = 1.0 - eps_mid;

    double gamma1 = 1.0 / std::sqrt(1.0 - beta * beta);
    double gamma2 = 1.0 / std::sqrt((2.0 - eps_mid) * eps_mid);

    double fractional_error = std::abs(gamma1 - gamma2) / gamma1;
    if (fractional_error <= target_error) {
      eps_high = eps_mid;
    } else {
      eps_low = eps_mid;
    }
  }

  double beta = 1.0 - eps_high;
  double gamma1_max = 1.0 / std::sqrt(1.0 - beta * beta);
  double gamma2_max = 1.0 / std::sqrt((2.0 - eps_high) * eps_high);
  double error_max = std::abs(gamma1_max - gamma2_max) / gamma1_max;

  std::cout << "\nMinimum epsilon = " << eps_high << "\n";
  std::cout << "gamma (Method 1) = " << gamma1_max << "\n";
  std::cout << "gamma (Method 2) = " << gamma2_max << "\n";
  std::cout << "Fractional Error = " << error_max << "\n";

  return 0;
}
