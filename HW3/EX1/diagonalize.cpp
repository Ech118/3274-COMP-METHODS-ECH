#include "diagonalize.h"
#include <cmath>
#include <iostream>
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
    // Sign error is corrected by Boudreau:
    coeffs.c1*=-1.0;
    
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
    std::cout << coeffs.c0 << " "
	      << coeffs.c1 << " "
	      << coeffs.c2 << " "
	      << std::endl;

    // Find eigenvalues using Newton's method with different initial guesses
    complex<double> lambda1 = newtonMethod(complex<double>(1.0, 0.0), coeffs);
    //complex<double> lambda2 = newtonMethod(complex<double>(0.0, 1.0), coeffs);
    //complex<double> lambda3 = newtonMethod(complex<double>(-1.0, 0.0), coeffs);

    // JB:  If you plotted the polynomial, you will notice that it only
    // intersects the x-axis once.  So Netwon's method will only give
    // on root.  But the other two roots can be inferred in the following
    // chain of logic:

    // 1.  Since there is only one real root, the other two must be complex.
    // 2.  Thinking of the diagonalized matrix, with the eigenvalues  along
    //     the diagonal
    //
    //   1     0        0
    //   0  lambda_2    0
    //   0     0     lambda_3
    //
    //  we can note that the determinent (same as that of the original matrix)
    //  is real, so lambda_1 and lambda_2 are complex conjugate pairs. If you
    //  happen to note that the trace of the original matrix is 1.0, you  will
    //  also see that lambda_1 = -lambda_2 = -lambda_1*   so these are both
    //  pure imaginary.  Finally, coming back to the determinant, since that
    //  is 1.0  (you did calculate it), we can say that the magnitude of
    //  lambda_1 and lambda_2 is "±i"
    //

    //   We can say more. The matrix is that of a rotation.  The eigenvector
    //   corresponding to eigenvalue +1 is the axis of rotation.  From
    //   tr(A)=1+2cos(Theta) we get cos(Theta)=0.0 so this is a ninety
    //   degree rotatation about the axis (1,1,1). 
     
    complex<double> lambda2 = +std::complex<double> (0.0,1.0);
    complex<double> lambda3 = -std::complex<double> (0.0,1.0);
   
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
