#include "decay.h"
#include <cmath>
#include <Eigen/Dense>
#include <unsupported/Eigen/MatrixFunctions>

using namespace Eigen;
using namespace std;

Matrix3d buildDecayMatrix(double hl1, double hl2, double hl3)
{
    double lambda1 = log(2.0) / hl1;
    double lambda2 = log(2.0) / hl2;
    double lambda3 = log(2.0) / hl3;

    Matrix3d Lambda;
    Lambda << -lambda1, 0, lambda3,
              lambda1, -lambda2, 0,
              0, lambda2, -lambda3;

    return Lambda;
}

Vector3d computePopulation(Matrix3d Lambda, Vector3d x0, double t)
{
    Matrix3d expLt = (Lambda * t).exp();
    return expLt * x0;
}
