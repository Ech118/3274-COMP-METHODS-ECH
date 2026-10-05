#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/SimpleIntegrator.h"
#include "QatGenericFunctions/CubicSplinePolynomial.h"
#include "QatGenericFunctions/Tanh.h"
#include "QatGenericFunctions/Cosh.h"
#include "QatGenericFunctions/Sinh.h"
#include "QatGenericFunctions/Sqrt.h"
#include "QatGenericFunctions/QuadratureRule.h"
#include "QatGenericFunctions/RombergIntegrator.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QSplitter>
#include <cmath>
#include <iostream>

using namespace Genfun;

int main(int argc, char * *argv) {
  // Automatically generated:-------------------------:

  std::string usage= std::string("usage: ") + argv[0]; 
  if (argc!=1) {
    std::cout << usage << std::endl;
  }


  QApplication app(argc, argv);

  QMainWindow window;
  QToolBar *toolBar = window.addToolBar("Tools");
  QAction *quitAction = toolBar->addAction("Quit");
  quitAction->setShortcut(QKeySequence("q"));
  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);

  // Part 1: tanh(x) integral

  // Analytical result: integral of tanh(x) = ln(cosh(x))
  // From 0 to 1: ln(cosh(1)) - ln(cosh(0)) = ln(cosh(1))
  double cosh1 = cosh(1.0);
  double analyticalTanh = log(cosh1);
  std::cout << "Analytical integral of tanh(x) from 0 to 1: " << analyticalTanh << std::endl;

  // Create functions
  Tanh tanh;

  // Plot for tanh
  PRectF rectTanh;
  rectTanh.setXmin(-3.0);  // log(1/256) ≈ -5.55, log(1/2) ≈ -0.3
  rectTanh.setXmax(0.5);
  rectTanh.setYmin(-16.0);  // log(small error)
  rectTanh.setYmax(-0.5);

  PlotView viewTanh(rectTanh);

  CubicSplinePolynomial rectangleSpline, trapezoidSpline, simpsonsSpline;

  // Compute errors for different n values
  for (unsigned int n = 2; n <= 256; n += 2) {
    double h = 1.0 / n;
    double logH = log(h);

    // Rectangle rule
    SimpleIntegrator rectIntegrator(0.0, 1.0, MidpointRule(), n);
    double rectResult = rectIntegrator(tanh);
    double rectError = fabs(rectResult - analyticalTanh);
    if (rectError > 1e-15) {
      rectangleSpline.addPoint(logH, log(rectError));
    }

    // Trapezoid rule
    SimpleIntegrator trapIntegrator(0.0, 1.0, TrapezoidRule(), n);
    double trapResult = trapIntegrator(tanh);
    double trapError = fabs(trapResult - analyticalTanh);
    if (trapError > 1e-15) {
      trapezoidSpline.addPoint(logH, log(trapError));
    }

    // Simpson's rule
    SimpleIntegrator simpIntegrator(0.0, 1.0, SimpsonsRule(), n);
    double simpResult = simpIntegrator(tanh);
    double simpError = fabs(simpResult - analyticalTanh);
    if (simpError > 1e-15) {
      simpsonsSpline.addPoint(logH, log(simpError));
    }
  }

  // Create plot functions with colors
  PlotFunction1D rectPlot(rectangleSpline);
  PlotFunction1D trapPlot(trapezoidSpline);
  PlotFunction1D simpPlot(simpsonsSpline);

  PlotFunction1D::Properties rectProps, trapProps, simpProps;

  rectProps.pen.setColor(Qt::red);
  rectProps.pen.setWidth(2);
  rectPlot.setProperties(rectProps);

  trapProps.pen.setColor(Qt::blue);
  trapProps.pen.setWidth(2);
  trapPlot.setProperties(trapProps);

  simpProps.pen.setColor(Qt::green);
  simpProps.pen.setWidth(2);
  simpPlot.setProperties(simpProps);

  viewTanh.add(&rectPlot);
  viewTanh.add(&trapPlot);
  viewTanh.add(&simpPlot);

  PlotStream titleTanhStream(viewTanh.titleTextEdit());
  titleTanhStream << PlotStream::Clear()
                  << PlotStream::Center()
                  << PlotStream::Family("Arial")
                  << PlotStream::Size(14)
                  << "Convergence: ∫₀¹ tanh(x) dx"
                  << PlotStream::EndP();

  PlotStream xLabelTanhStream(viewTanh.xLabelTextEdit());
  xLabelTanhStream << PlotStream::Clear()
                   << PlotStream::Center()
                   << PlotStream::Family("Arial")
                   << PlotStream::Size(12)
                   << "log(h) where h = 1/n"
                   << PlotStream::EndP();

  PlotStream yLabelTanhStream(viewTanh.yLabelTextEdit());
  yLabelTanhStream << PlotStream::Clear()
                   << PlotStream::Center()
                   << PlotStream::Family("Arial")
                   << PlotStream::Size(12)
                   << "log(|error|)"
                   << PlotStream::EndP();


  // Part 2: sqrt(coth(x)) integral

  // For Analytical I use Romberg integration as "truth"
  Cosh cosh;
  Sinh sinh;
  GENFUNCTION coth = cosh / sinh;
  GENFUNCTION sqrtCoth = Sqrt()(coth);

  // Use high precision integrator for analytical value
  RombergIntegrator rombergIntegrator(0.01, 0.99);
  rombergIntegrator.setEpsilon(1e-10);
  double analyticalSqrtCoth = rombergIntegrator(sqrtCoth);
  std::cout << "Analytical integral of sqrt(coth(x)) from 0.01 -> 0.99: " << analyticalSqrtCoth << std::endl;

  // Plot for sqrt(coth)
  PRectF rectSqrtCoth;
  rectSqrtCoth.setXmin(-3.0);
  rectSqrtCoth.setXmax(0.5);
  rectSqrtCoth.setYmin(-16.0);
  rectSqrtCoth.setYmax(-0.5);

  PlotView viewSqrtCoth(rectSqrtCoth);

  CubicSplinePolynomial rectangleSpline2, trapezoidSpline2, simpsonsSpline2;

  for (unsigned int n = 2; n <= 256; n += 2) {
    double h = 1.0 / n;
    double logH = log(h);

    // Rectangle rule
    SimpleIntegrator rectIntegrator(0.01, 0.99, MidpointRule(), n);
    double rectResult = rectIntegrator(sqrtCoth);
    double rectError = fabs(rectResult - analyticalSqrtCoth);
    if (rectError > 1e-15) {
      rectangleSpline2.addPoint(logH, log(rectError));
    }

    // Trapezoid rule
    SimpleIntegrator trapIntegrator(0.01, 0.99, TrapezoidRule(), n);
    double trapResult = trapIntegrator(sqrtCoth);
    double trapError = fabs(trapResult - analyticalSqrtCoth);
    if (trapError > 1e-15) {
      trapezoidSpline2.addPoint(logH, log(trapError));
    }

    // Simpson's rule
    SimpleIntegrator simpIntegrator(0.01, 0.99, SimpsonsRule(), n);
    double simpResult = simpIntegrator(sqrtCoth);
    double simpError = fabs(simpResult - analyticalSqrtCoth);
    if (simpError > 1e-15) {
      simpsonsSpline2.addPoint(logH, log(simpError));
    }
  }

  // Create plot functions with colors
  PlotFunction1D rectPlot2(rectangleSpline2);
  PlotFunction1D trapPlot2(trapezoidSpline2);
  PlotFunction1D simpPlot2(simpsonsSpline2);

  PlotFunction1D::Properties rectProps2, trapProps2, simpProps2;

  rectProps2.pen.setColor(Qt::red);
  rectProps2.pen.setWidth(2);
  rectPlot2.setProperties(rectProps2);

  trapProps2.pen.setColor(Qt::blue);
  trapProps2.pen.setWidth(2);
  trapPlot2.setProperties(trapProps2);

  simpProps2.pen.setColor(Qt::green);
  simpProps2.pen.setWidth(2);
  simpPlot2.setProperties(simpProps2);

  viewSqrtCoth.add(&rectPlot2);
  viewSqrtCoth.add(&trapPlot2);
  viewSqrtCoth.add(&simpPlot2);

  PlotStream titleSqrtCothStream(viewSqrtCoth.titleTextEdit());
  titleSqrtCothStream << PlotStream::Clear()
                      << PlotStream::Center()
                      << PlotStream::Family("Arial")
                      << PlotStream::Size(14)
                      << "Convergence: ∫0-1 √(coth(x)) dx"
                      << PlotStream::EndP();

  PlotStream xLabelSqrtCothStream(viewSqrtCoth.xLabelTextEdit());
  xLabelSqrtCothStream << PlotStream::Clear()
                       << PlotStream::Center()
                       << PlotStream::Family("Arial")
                       << PlotStream::Size(12)
                       << "log(h) where h = 1/n"
                       << PlotStream::EndP();

  PlotStream yLabelSqrtCothStream(viewSqrtCoth.yLabelTextEdit());
  yLabelSqrtCothStream << PlotStream::Clear()
                       << PlotStream::Center()
                       << PlotStream::Family("Arial")
                       << PlotStream::Size(12)
                       << "log(|error|)"
                       << PlotStream::EndP();

  QSplitter *splitter = new QSplitter(Qt::Horizontal);
  splitter->addWidget(&viewTanh);
  splitter->addWidget(&viewSqrtCoth);
  window.setCentralWidget(splitter);

  // Show tanh plot first
  window.show();
  app.exec();
  return 0;
}
