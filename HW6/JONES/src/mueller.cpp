#include "MUELLERCALC/StokesVector.h"
#include "MUELLERCALC/MuellerMatrix.h"
#include <iostream>
#include <cmath>

void report(const std::string & label, const StokesVector & s) {
  std::cout << label << s << "   intensity= " << s.intensity()
            << "   DOP= " << s.degreeOfPolarization() << std::endl;
}

int main (int argc, char ** argv) {

  // 1) circular polarizer
  StokesVector  v0(StokesVector::Horizontal);
  MuellerMatrix PH(MuellerMatrix::FastHorizontal);
  MuellerMatrix DG(MuellerMatrix::Diagonal);
  MuellerMatrix PV(MuellerMatrix::FastVertical);

  MuellerMatrix CP=PH*DG*PV;
  std::cout << "1) Circular polarizer, horizontal light in" << std::endl;
  std::cout << "Filter= " << std::endl << CP << std::endl;
  report("Initial= ", v0);
  report("Final=   ", CP*v0);
  std::cout << std::endl;

  // 2) Unpolarized light through a horizontal polarizer
  StokesVector  u(StokesVector::Unpolarized);
  MuellerMatrix H(MuellerMatrix::Horizontal);
  std::cout << "2) Unpolarized light through a horizontal polarizer" << std::endl;
  report("Initial= ", u);
  report("Final=   ", H*u);
  std::cout << std::endl;

  // 3) Malus's law: unpolarized -> horizontal polarizer -> polarizer at theta
  std::cout << "3) Malus's law, expected I = 0.5 cos^2(theta)" << std::endl;
  for (double deg : {0.0, 30.0, 45.0, 60.0, 90.0}) {
    double theta=deg*M_PI/180;
    StokesVector out=MuellerMatrix::LinearPolarizer(theta)*H*u;
    std::cout << "theta= " << deg << "   I= " << out.intensity()
              << "   0.5cos^2= " << 0.5*cos(theta)*cos(theta) << std::endl;
  }
  std::cout << std::endl;

  // 4) Partially polarized light: incoherent mix of unpolarized and horizontal
  StokesVector  p=0.6*u + 0.4*StokesVector(StokesVector::Horizontal);
  MuellerMatrix V(MuellerMatrix::Vertical);
  std::cout << "4) Partially polarized light (60% unpolarized + 40% horizontal)" << std::endl;
  report("Initial=              ", p);
  report("Horizontal polarizer= ", H*p);
  report("Vertical polarizer=   ", V*p);
  std::cout << std::endl;

  // 5) Three polarizers: H, then diagonal, then V, transmit 1/8 of unpolarized light
  std::cout << "5) Unpolarized light through H, diagonal, V polarizers" << std::endl;
  report("Crossed H,V=      ", V*H*u);
  report("H,diagonal,V=     ", V*DG*H*u);
  std::cout << std::endl;

  // 6) A depolarizer destroys the polarization but keeps the intensity
  MuellerMatrix D(MuellerMatrix::Depolarizer);
  std::cout << "6) Horizontal light through a depolarizer" << std::endl;
  report("Initial= ", v0);
  report("Final=   ", D*v0);

  return 0;
}
