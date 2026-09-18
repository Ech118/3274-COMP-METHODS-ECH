#include "schrodinger.h"
#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/F1D.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <complex>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace Genfun;

const double A = 1.0;
const double V_OVER_E = 1.5;

// Solution state, filled in main()
WaveNumbers waveNumbers;
VectorXcd amps; // [r, A2, B2, A3, B3, A4, B4, t]

// |psi(x)|^2 with an incident wave exp(i k0 x) of unit amplitude
double Psi2(double x)
{
  const std::complex<double> i(0, 1);
  std::complex<double> psi;

  if (x < -2.0 * A)
  {
    psi = std::exp(i * waveNumbers.k0 * x) + amps(0) * std::exp(-i * waveNumbers.k0 * x);
  }
  else if (x < -A)
  {
    psi = amps(1) * std::exp(i * waveNumbers.k2 * x) + amps(2) * std::exp(-i * waveNumbers.k2 * x);
  }
  else if (x < A)
  {
    psi = amps(3) * std::exp(i * waveNumbers.k3 * x) + amps(4) * std::exp(-i * waveNumbers.k3 * x);
  }
  else if (x < 2.0 * A)
  {
    psi = amps(5) * std::exp(i * waveNumbers.k2 * x) + amps(6) * std::exp(-i * waveNumbers.k2 * x);
  }
  else
  {
    psi = amps(7) * std::exp(i * waveNumbers.k0 * x);
  }

  return std::norm(psi);
}


int main (int argc, char * * argv)
{
  std::string usage = std::string("usage: ") + argv[0] + " <ka>\n"
    "  ka: k0*a for the incoming wave, EX: 0.5, 1, 2, 3, 4";
  if (argc != 2)
  {
    std::cout << usage << std::endl;
    return 1;
  }
  double ka = 0.0;
  try
  {
    ka = std::stod(argv[1]);
  }
  catch (...)
  {
    ka = 0.0;
  }
  if (ka <= 0.0)
  {
    std::cout << usage << std::endl;
    return 1;
  }

  // hbar = m = 1, so E = (k0)^2 / 2 and V0 = 1.5 E
  double E = ka * ka / (2.0 * A * A);
  double V0 = V_OVER_E * E;
  waveNumbers = calcWaveNumbers(E, V0, A);
  amps = solveAmplitudes(E, V0, A);
  TransmissionData data = solveSchrodinger(E, V0, A);

  QApplication     app(argc,argv);
  
  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");
  
  quitAction->setShortcut(QKeySequence("q"));
  
  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);

  // Plot from -4a to 4a; the barrier regions sit between -2a and 2a
  PRectF rect;
  rect.setXmin(-4.0 * A);
  rect.setXmax(4.0 * A);
  rect.setYmin(0.0);
  rect.setYmax(4.0);

  PlotView view(rect);
  window.setCentralWidget(&view);

  F1D F(Psi2);
  PlotFunction1D pPsi = F;
  {
    PlotFunction1D::Properties prop;
    prop.pen.setWidth(3);
    pPsi.setProperties(prop);
  }
  view.add(&pPsi);

  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center() 
	      << PlotStream::Family("Arial") 
	      << PlotStream::Size(16)
	      << "V/E = 1.5,  ka = " << ka
	      << ",  T = " << PlotStream::SetPrecision(3) << data.T
	      << PlotStream::EndP();
  
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << "x/a"
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << "|psi|"
	       << PlotStream::Super() << "2"
	       << PlotStream::EndP();
  
  window.show();
  app.exec();
  return 0;
}
