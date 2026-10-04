#ifndef _MUELLERMATRIX_H_
#define _MUELLERMATRIX_H_
#include "MUELLERCALC/StokesVector.h"
#include <Eigen/Dense>
#include <iostream>
class MuellerMatrix {

 public:

  enum Type { Horizontal,Vertical,Diagonal,
	      Antidiagonal,Right,Left,
	      Identity, FastHorizontal, FastVertical,
	      Depolarizer};

  // Construct the identity matrix:
  inline MuellerMatrix()=default;

  // Construct a predefined type:
  inline MuellerMatrix(Type type);

  // Construct on a 4x4 matrix:
  inline MuellerMatrix(const Eigen::Matrix4d & m);

  // Linear polarizer with transmission axis at angle theta (radians) from horizontal:
  inline static MuellerMatrix LinearPolarizer(double theta);

  // Access to individual elements:
  inline double & operator () (unsigned int i, unsigned int j);
  inline const double & operator () (unsigned int i, unsigned int j) const;

  // The underlying Eigen matrix:
  inline const Eigen::Matrix4d & matrix() const;

 private:

  Eigen::Matrix4d m_a{Eigen::Matrix4d::Identity()};

};

inline std::ostream & operator << (std::ostream & o, const MuellerMatrix & m);
inline MuellerMatrix operator* (const MuellerMatrix & m1, const MuellerMatrix & m2);
inline StokesVector  operator* (const MuellerMatrix & m,  const StokesVector & v);
#include "MUELLERCALC/MuellerMatrix.icc"
#endif
