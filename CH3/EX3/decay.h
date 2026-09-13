#ifndef DECAY_H
#define DECAY_H

#include <Eigen/Dense>

using namespace Eigen;

Matrix3d buildDecayMatrix(double half_life_1, double half_life_2, double half_life_3);

Vector3d computePopulation(Matrix3d Lambda, Vector3d x0, double t);

#endif
