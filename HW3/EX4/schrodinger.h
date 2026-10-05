#ifndef SCHRODINGER_H
#define SCHRODINGER_H

#include <Eigen/Dense>
#include <complex>

using namespace Eigen;
using namespace std;

struct WaveNumbers
{
    complex<double> k0;  // region 1 and 5 (V=0)
    complex<double> k2;  // region 2 and 4 (V=V0)
    complex<double> k3;  // region 3 (V=2V0)
};

struct TransmissionData
{
    double R;  // reflection coefficient
    double T;  // transmission coefficient
};

// Calculate wave numbers for given E, V0
WaveNumbers calcWaveNumbers(double E, double V0, double a);

// Solve for reflection and transmission coefficients
TransmissionData solveSchrodinger(double E, double V0, double a);

#endif
