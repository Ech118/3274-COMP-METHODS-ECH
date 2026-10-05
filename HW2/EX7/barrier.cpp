#include <iostream>
#include <cmath>

int main() {
  std::cout << "Quantum Tunneling Through a Potential Barrier\n";
  std::cout << "\n";

  double barrier_width = 1.0;
  double V = 1.0;

  double ratios[] = {0.5, 0.8, 1.0, 1.2, 1.5, 2.0};

  for (double ratio : ratios) {
    double E = ratio * V;
    double R, T, param;

    if (E < V) {
      double kappa = std::sqrt(V - E);
      double phase = kappa * barrier_width;
      double sinh_phase = std::sinh(phase);
      double sinh2 = sinh_phase * sinh_phase;

      R = sinh2 / (1.0 + sinh2);
      T = 1.0 / (1.0 + sinh2);
      param = kappa;
    } else if (E > V) {
      double q = std::sqrt(E - V);
      double phase = q * barrier_width;
      double sin_phase = std::sin(phase);
      double sin2 = sin_phase * sin_phase;

      R = sin2 / (1.0 + sin2);
      T = 1.0 / (1.0 + sin2);
      param = q;
    } else {
      R = 0.0;
      T = 1.0;
      param = 0.0;
    }

    std::cout << "E/V = " << ratio << "\n";
    std::cout << "  kappa = " << param << "\n";
    std::cout << "  Reflection (R) = " << R << "\n";
    std::cout << "  Transmission (T) = " << T << "\n";
    std::cout << "  R + T = " << (R + T) << "\n\n";
  }

  std::cout << "R + T = 1.0 (energy conservation)\n";
  std::cout << "For E < V -> particles can tunnel through the barrier (T > 0)\n";
  std::cout << "For E > V -> quantum interference affects transmission\n";

  return 0;
}
