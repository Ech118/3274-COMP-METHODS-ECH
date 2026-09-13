#include "schrodinger.h"
#include <cmath>

using namespace Eigen;
using namespace std;

// Setting h_bar = 1, m = 1 for simplicity
const double HBAR = 1.0;
const double MASS = 1.0;

WaveNumbers calcWaveNumbers(double E, double V0, double a)
{
    WaveNumbers kn;

    // k0 for region 1,5 (V=0): k0 = sqrt(2mE/hbar^2)
    kn.k0 = complex<double>(sqrt(2.0 * MASS * E / (HBAR * HBAR)), 0.0);

    // k2 for region 2,4 (V=V0): k2 = sqrt(2m(E-V0)/hbar^2)
    double arg2 = 2.0 * MASS * (E - V0) / (HBAR * HBAR);
    if (arg2 >= 0)
    {
        kn.k2 = complex<double>(sqrt(arg2), 0.0);
    }
    else
    {
        kn.k2 = complex<double>(0.0, sqrt(-arg2));
    }

    // k3 for region 3 (V=2V0): k3 = sqrt(2m(E-2V0)/hbar^2)
    double arg3 = 2.0 * MASS * (E - 2.0 * V0) / (HBAR * HBAR);
    if (arg3 >= 0)
    {
        kn.k3 = complex<double>(sqrt(arg3), 0.0);
    }
    else
    {
        kn.k3 = complex<double>(0.0, sqrt(-arg3));
    }

    return kn;
}

TransmissionData solveSchrodinger(double E, double V0, double a)
{
    TransmissionData result;

    WaveNumbers kn = calcWaveNumbers(E, V0, a);

    // Build 8x8 matrix for boundary matching
    // Unknowns: [r, A2, B2, A3, B3, A4, B4, t]
    MatrixXcd M = MatrixXcd::Zero(8, 8);
    VectorXcd b = VectorXcd::Zero(8);

    complex<double> i(0, 1);
    double x1 = -2.0 * a;  // boundary 1
    double x2 = -a;        // boundary 2
    double x3 = a;         // boundary 3
    double x4 = 2.0 * a;   // boundary 4

    // Boundary 1 at x = -2a: psi1 = psi2
    complex<double> exp_k0_x1 = exp(i * kn.k0 * x1);
    complex<double> exp_k2_x1 = exp(i * kn.k2 * x1);
    M(0, 0) = exp(-i * kn.k0 * x1);               // r coefficient
    M(0, 1) = -exp_k2_x1;                         // A2
    M(0, 2) = -exp(-i * kn.k2 * x1);              // B2
    b(0) = -exp_k0_x1;                            // RHS from incident wave

    // Boundary 1: dpsi1/dx = dpsi2/dx
    M(1, 0) = -i * kn.k0 * exp(-i * kn.k0 * x1); // r coefficient
    M(1, 1) = -i * kn.k2 * exp_k2_x1;            // A2
    M(1, 2) = i * kn.k2 * exp(-i * kn.k2 * x1);  // B2
    b(1) = -i * kn.k0 * exp_k0_x1;               // RHS

    // Boundary 2 at x = -a: psi2 = psi3
    complex<double> exp_k2_x2 = exp(i * kn.k2 * x2);
    complex<double> exp_k3_x2 = exp(i * kn.k3 * x2);
    M(2, 1) = exp_k2_x2;                         // A2
    M(2, 2) = exp(-i * kn.k2 * x2);              // B2
    M(2, 3) = -exp_k3_x2;                        // A3
    M(2, 4) = -exp(-i * kn.k3 * x2);             // B3
    b(2) = 0.0;

    // Boundary 2: dpsi2/dx = dpsi3/dx
    M(3, 1) = i * kn.k2 * exp_k2_x2;             // A2
    M(3, 2) = -i * kn.k2 * exp(-i * kn.k2 * x2); // B2
    M(3, 3) = -i * kn.k3 * exp_k3_x2;            // A3
    M(3, 4) = i * kn.k3 * exp(-i * kn.k3 * x2);  // B3
    b(3) = 0.0;

    // Boundary 3 at x = a: psi3 = psi4
    complex<double> exp_k3_x3 = exp(i * kn.k3 * x3);
    complex<double> exp_k2_x3 = exp(i * kn.k2 * x3);
    M(4, 3) = exp_k3_x3;                         // A3
    M(4, 4) = exp(-i * kn.k3 * x3);              // B3
    M(4, 5) = -exp_k2_x3;                        // A4
    M(4, 6) = -exp(-i * kn.k2 * x3);             // B4
    b(4) = 0.0;

    // Boundary 3: dpsi3/dx = dpsi4/dx
    M(5, 3) = i * kn.k3 * exp_k3_x3;             // A3
    M(5, 4) = -i * kn.k3 * exp(-i * kn.k3 * x3); // B3
    M(5, 5) = -i * kn.k2 * exp_k2_x3;            // A4
    M(5, 6) = i * kn.k2 * exp(-i * kn.k2 * x3);  // B4
    b(5) = 0.0;

    // Boundary 4 at x = 2a: psi4 = psi5
    complex<double> exp_k2_x4 = exp(i * kn.k2 * x4);
    complex<double> exp_k0_x4 = exp(i * kn.k0 * x4);
    M(6, 5) = exp_k2_x4;                         // A4
    M(6, 6) = exp(-i * kn.k2 * x4);              // B4
    M(6, 7) = -exp_k0_x4;                        // t
    b(6) = 0.0;

    // Boundary 4: dpsi4/dx = dpsi5/dx
    M(7, 5) = i * kn.k2 * exp_k2_x4;             // A4
    M(7, 6) = -i * kn.k2 * exp(-i * kn.k2 * x4); // B4
    M(7, 7) = -i * kn.k0 * exp_k0_x4;            // t
    b(7) = 0.0;

    // Solve the system
    VectorXcd amplitudes = M.householderQr().solve(b);

    complex<double> r = amplitudes(0);
    complex<double> t = amplitudes(7);

    // Calculate R and T
    result.R = abs(r) * abs(r);
    result.T = abs(t) * abs(t);

    return result;
}
