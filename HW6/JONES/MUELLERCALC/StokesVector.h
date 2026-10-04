#ifndef _STOKESVECTOR_H_
#define _STOKESVECTOR_H_
#include <Eigen/Dense>
#include <iostream>
class StokesVector {

 public:

  enum Type {Horizontal,Vertical,Diagonal,Antidiagonal,Right,Left,Unpolarized};

  // Construct a zero vector (no light):
  inline StokesVector()=default;

  // Construct a predefined type, with intensity S0:
  inline StokesVector(Type type, double intensity=1.0);

  // Construct on the four Stokes parameters:
  inline StokesVector(double s0, double s1, double s2, double s3);

  // Construct on an Eigen 4-vector:
  inline StokesVector(const Eigen::Vector4d & s);

  // Intensity (S0) and degree of polarization sqrt(S1^2+S2^2+S3^2)/S0:
  inline double intensity() const;
  inline double degreeOfPolarization() const;

  // Access to individual elements:
  inline double & operator () (unsigned int i);
  inline const double & operator () (unsigned int i) const;

  // The underlying Eigen vector:
  inline const Eigen::Vector4d & vector() const;

 private:

  Eigen::Vector4d m_s{Eigen::Vector4d::Zero()};

};

inline std::ostream & operator << (std::ostream & o, const StokesVector & v);

// Incoherent superposition of beams, and scaling:
inline StokesVector operator+ (const StokesVector & v1, const StokesVector & v2);
inline StokesVector operator* (const StokesVector & v,  double c);
inline StokesVector operator* (double c,                const StokesVector & v);
#include "MUELLERCALC/StokesVector.icc"
#endif
