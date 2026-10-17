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
#include <QSplitter>
#include <cstdlib>
#include <iostream>
#include <string>
#include <numbers>
#include <cmath>
#include <random>

// Transformation 1: f(x) = xmin (xmax/xmin)^x
const double xMinT = 1.0;
const double xMaxT = 10.0;

// Transformation 2: f(x) = sqrt(M^2 + M Gamma tan(M Gamma x))
const double M     = 100.0;
const double Gamma = 10.0;

// rho(y) = dx/dy for y = xmin (xmax/xmin)^x  ->  1 / (y ln(xmax/xmin)), y in [xmin, xmax]
double logDistribution(double y)
{
  if (y < xMinT || y > xMaxT)
  {
    return 0.0;
  }
  return 1.0 / (y * std::log(xMaxT / xMinT));
}

// s = y^2 = M^2 + M Gamma t with t Cauchy distributed  ->  relativistic Breit-Wigner in y
double breitWigner(double y)
{
  double s = y * y - M * M;
  return (2.0 * y / std::numbers::pi) * M * Gamma / (s * s + M * M * Gamma * Gamma);
}

// Title and axis labels for one PlotView
void label(PlotView & view, const std::string & title, const std::string & xLabel, const std::string & yLabel)
{
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center()
	      << PlotStream::Family("Arial")
	      << PlotStream::Size(16)
	      << title
	      << PlotStream::EndP();

  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << xLabel
	       << PlotStream::EndP();

  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << yLabel
	       << PlotStream::EndP();
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

  // Two plots side by side
  PRectF rect1;
  rect1.setXmin(xMinT);
  rect1.setXmax(xMaxT);
  rect1.setYmin(0.0);
  rect1.setYmax(0.5);

  PRectF rect2;
  rect2.setXmin(50.0);
  rect2.setXmax(150.0);
  rect2.setYmin(0.0);
  rect2.setYmax(0.07);

  // Splitter: the two views share the window, divider can be dragged to resize
  QSplitter splitter(Qt::Horizontal);
  PlotView  view1(rect1);
  PlotView  view2(rect2);
  splitter.addWidget(&view1);
  splitter.addWidget(&view2);
  window.setCentralWidget(&splitter);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> uniform(0.0, 1.0);

  const int N     = 10000000;
  const int nBins = 100;

  Hist1D hist1("f(x) = xmin (xmax/xmin)^x", nBins, rect1.xmin(), rect1.xmax());
  Hist1D hist2("f(x) = sqrt(M^2 + M Gamma tan(M Gamma x))", nBins, rect2.xmin(), rect2.xmax());
  const double binWidth1 = (rect1.xmax() - rect1.xmin()) / nBins;
  const double binWidth2 = (rect2.xmax() - rect2.xmin()) / nBins;

  int imaginary = 0;
  for (int i = 0; i < N; i++)
  {
    double x = uniform(gen);

    double y1 = xMinT * std::pow(xMaxT / xMinT, x);
    hist1.accumulate(y1, 1.0/(N * binWidth1));

    // When M^2 + M Gamma tan(...) < 0 the transformation has no real value; skip
    // the sample but keep it in N so the histogram matches rho(y) restricted to y > 0
    double s = M * M + M * Gamma * std::tan(M * Gamma * x);
    if (s < 0.0)
    {
      imaginary++;
      continue;
    }
    hist2.accumulate(std::sqrt(s), 1.0/(N * binWidth2));
  }

  std::cout << "Fraction with M^2 + M Gamma tan < 0: " << imaginary / double(N)
            << " (Cauchy prediction " << (std::numbers::pi/2 - std::atan(M/Gamma)) / std::numbers::pi
            << ")" << std::endl;

  Genfun::F1D f1(logDistribution);
  Genfun::F1D f2(breitWigner);
  PlotFunction1D p1(f1);
  PlotFunction1D p2(f2);
  PlotHist1D pHist1(hist1);
  PlotHist1D pHist2(hist2);

  view1.add(&pHist1);
  view1.add(&p1);
  view2.add(&pHist2);
  view2.add(&p2);

  label(view1, "f(x) = xmin (xmax/xmin)^x", "y", "ρ(y) = 1 / (y ln 10)");
  label(view2, "f(x) = √(M² + MΓ tan(MΓx))", "y", "ρ(y), relativistic Breit–Wigner");

  window.resize(1300, 550);
  window.show();
  app.exec();
  return 0;
}
