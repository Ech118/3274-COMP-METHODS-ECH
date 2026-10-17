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
#include <numbers>
#include <cmath>
#include <random>

double distribution(double v)
{
  // m = 1, tao = 1
  double scale = std::sqrt(2/(std::numbers::pi));
  double exponent = std::exp(-(v * v)/2);
  return scale * exponent * v * v;
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
  rect.setXmin(0.0);
  rect.setXmax(5.0);
  rect.setYmin(0.0);
  rect.setYmax(0.7);
  

  PlotView view(rect);
  window.setCentralWidget(&view);

  std::random_device rd;
  std::mt19937 gen(rd());

  std::gamma_distribution<double> gamma(1.5, 1.0);

  Genfun::F1D f1(distribution);
  PlotFunction1D p1(f1);

  const int    N        = 10000000;
  const int    nBins    = 100;
  const double xMin     = 0.0;
  const double xMax     = 5.0;
  const double binWidth = (xMax - xMin) / nBins;

  Hist1D hist("Maxwell samples", nBins, xMin, xMax);


  for (int i = 0; i < N; i++)
  {
    double u = gamma(gen);
    double x = std::sqrt(2.0 * u);
    hist.accumulate(x, 1.0/(N * binWidth));
  }

  PlotHist1D pHist(hist);

  view.add(&pHist);
  view.add(&p1);
  
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center() 
	      << PlotStream::Family("Arial") 
	      << PlotStream::Size(16)
	      << "Maxwell Speed Distribution: exact vs. "
	      << "std::gamma_distribution (N = " << N << ")"
	      << PlotStream::EndP();
  
  
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << "x = v / √(τ/m)"
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << "ρ(x)"
	       << PlotStream::EndP();
  
  
  
  window.show();
  app.exec();
  return 0;
}

