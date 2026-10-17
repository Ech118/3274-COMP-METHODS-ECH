#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatPlotting/PlotHist1D.h"
#include "QatDataAnalysis/Hist1D.h"
#include "QatGenericFunctions/F1D.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>
#include <random>

// Legendre polynomials P1, P3, P5, P7
double P1(double x) { return x; }
double P3(double x) { return (5*std::pow(x,3) - 3*x) / 2; }
double P5(double x) { return (63*std::pow(x,5) - 70*std::pow(x,3) + 15*x) / 8; }
double P7(double x) { return (429*std::pow(x,7) - 693*std::pow(x,5) + 315*std::pow(x,3) - 35*x) / 16; }

// Angular distribution rho(cos theta); already normalized to 1 on [-1, 1]
double distribution(double c)
{
  double amp = std::sqrt(3.0/8.0)   * P1(c)
             - std::sqrt(7.0/6.0)   * P3(c)
             + std::sqrt(11.0/24.0) * P5(c)
             - std::sqrt(15.0/6.0)  * P7(c);
  return amp * amp;
}

int main (int argc, char * * argv) {

  // Automatically generated:-------------------------:

  std::string usage= std::string("usage: ") + argv[0]; 
  if (argc!=1) {
    std::cout << usage << std::endl;
  }


  QApplication     app(argc,argv);
  
  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");
  
  quitAction->setShortcut(QKeySequence("q"));
  
  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);
  
  PRectF rect;
  rect.setXmin(-1.0);
  rect.setXmax(1.0);
  rect.setYmin(0.0);
  rect.setYmax(2.0);
  

  PlotView view(rect);
  window.setCentralWidget(&view);

  std::random_device rd;
  std::mt19937 gen(rd());

  Genfun::F1D f1(distribution);
  PlotFunction1D p1(f1);

  const int    N        = 10000000;
  const int    nBins    = 100;
  const double cMin     = -1.0;
  const double cMax     = 1.0;
  const double binWidth = (cMax - cMin) / nBins;

  Hist1D hist("Decay angle samples", nBins, cMin, cMax);

  // Envelope height: scan for the maximum of rho, plus a 1% safety margin
  double rhoMax = 0.0;
  for (int i = 0; i <= 10000; i++)
  {
    rhoMax = std::max(rhoMax, distribution(cMin + i * (cMax - cMin) / 10000));
  }
  rhoMax *= 1.01;

  // Accept-reject: pick a point uniformly under the box [-1,1] x [0,rhoMax],
  // keep its c only if the point lies under the curve rho(c)
  std::uniform_real_distribution<double> uc(cMin, cMax);
  std::uniform_real_distribution<double> uy(0.0, rhoMax);

  long tries = 0;
  int  kept  = 0;
  while (kept < N)
  {
    double c = uc(gen);
    double y = uy(gen);
    tries++;
    if (y < distribution(c))
    {
      hist.accumulate(c, 1.0/(N * binWidth));
      kept++;
    }
  }

  std::cout << "Acceptance efficiency: " << kept / double(tries)
            << " (expected 1/(2*rhoMax) = " << 1.0/(2.0*rhoMax) << ")" << std::endl;

  PlotHist1D pHist(hist);

  view.add(&pHist);
  view.add(&p1);
  
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center() 
	      << PlotStream::Family("Arial") 
	      << PlotStream::Size(16)
	      << "Polarized decay: exact vs. accept-reject (N = " << N << ")"
	      << PlotStream::EndP();
  
  
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << "cos θ"
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << "ρ(cos θ)"
	       << PlotStream::EndP();
  
  
  
  window.show();
  app.exec();
  return 0;
}

