#include "diagonalize.h"
#include <cmath>

using namespace Eigen;
using namespace std;

struct PolyCoeffs {
    complex<double> c2, c1, c0;
};

PolyCoeffs getCharacteristicPolyCoeffs(Matrix3cd A)
{
    PolyCoeffs coeffs;
    coeffs.c2 = A.trace();

    // Sum of principal 2x2 minors
    coeffs.c1 = A(0, 0) * A(1, 1) - A(0, 1) * A(1, 0) +
                A(0, 0) * A(2, 2) - A(0, 2) * A(2, 0) +
                A(1, 1) * A(2, 2) - A(1, 2) * A(2, 1);

    coeffs.c0 = A.determinant();
    return coeffs;
}

complex<double> evalPoly(complex<double> lambda, PolyCoeffs coeffs)
{
    return -lambda * lambda * lambda + coeffs.c2 * lambda * lambda +
           coeffs.c1 * lambda + coeffs.c0;
}

complex<double> evalPolyDerivative(complex<double> lambda, PolyCoeffs coeffs)
{
    return -3.0 * lambda * lambda + 2.0 * coeffs.c2 * lambda + coeffs.c1;
}

complex<double> newtonMethod(complex<double> initial_guess, PolyCoeffs coeffs,
                             int max_iterations = 100, double tolerance = 1e-10)
{
    complex<double> x = initial_guess;
    for (int i = 0; i < max_iterations; i++)
    {
        complex<double> fx = evalPoly(x, coeffs);
        complex<double> fpx = evalPolyDerivative(x, coeffs);

        if (abs(fpx) < 1e-15)
        {
            break;
        }

        complex<double> x_new = x - fx / fpx;
        if (abs(x_new - x) < tolerance)
        {
            return x_new;
        }
        x = x_new;
    }
    return x;
}

Vector3cd findEigenvector(Matrix3cd A, complex<double> eigenvalue)
{
    Matrix3cd M = A - eigenvalue * Matrix3cd::Identity();

    HouseholderQR<Matrix3cd> qr(M);
    Matrix3cd Q = qr.householderQ() * Matrix3cd::Identity();
    Vector3cd v = Q.col(2);

    v.normalize();
    return v;
}

DiagonalResult diagonalize(Matrix3cd A)
{
    DiagonalResult result;

    PolyCoeffs coeffs = getCharacteristicPolyCoeffs(A);

    // Find eigenvalues using Newton's method with different initial guesses
    complex<double> lambda1 = newtonMethod(complex<double>(1.0, 0.0), coeffs);
    complex<double> lambda2 = newtonMethod(complex<double>(0.0, 1.0), coeffs);
    complex<double> lambda3 = newtonMethod(complex<double>(-1.0, 0.0), coeffs);

    result.eigenvalues[0] = lambda1;
    result.eigenvalues[1] = lambda2;
    result.eigenvalues[2] = lambda3;

    // Find and normalize eigenvectors
    Vector3cd v1 = findEigenvector(A, lambda1);
    Vector3cd v2 = findEigenvector(A, lambda2);
    Vector3cd v3 = findEigenvector(A, lambda3);

    // Assemble into matrix (columns are eigenvectors)
    result.eigenvectors.col(0) = v1;
    result.eigenvectors.col(1) = v2;
    result.eigenvectors.col(2) = v3;

    return result;
}
