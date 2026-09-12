#include "diagonalize.h"
#include <iostream>

using namespace Eigen;
using namespace std;

int main()
{
    Matrix3cd A;
    A << 0.333333, -0.244017, 0.910684,
         0.910684, 0.33333333, -0.244017,
         -0.244017, 0.910684, 0.333333;

    DiagonalResult result = diagonalize(A);

    cout << "Eigenvalues:" << endl;
    for (int i = 0; i < 3; i++)
    {
        cout << result.eigenvalues[i] << endl;
    }

    cout << "\nEigenvectors:" << endl;
    cout << result.eigenvectors << endl;

    return 0;
}
