#include <Eigen/Dense>
#include <cmath>

using namespace Eigen;

Matrix3d get_mat()
{
  // Edge vecs
  Vector3d edge_a(3.0, 0.0, 0.0);
  Vector3d edge_b(0.5, 2.0, 0.0);
  Vector3d edge_c(0.3, 0.2, 1.5);

  
  Matrix3d mat;
  mat.col(0) = edge_a;
  mat.col(1) = edge_b;
  mat.col(2) = edge_c;

  return mat;
}

// Determinant 
double det()
{
  return std::abs(get_mat().determinant());
}

// Triple product: a * (b × c)
double trip_prod()
{
  Matrix3d mat = get_mat();
  Vector3d a = mat.col(0);
  Vector3d b = mat.col(1);
  Vector3d c = mat.col(2);
  
  return std::abs(a.dot(b.cross(c)));
}



