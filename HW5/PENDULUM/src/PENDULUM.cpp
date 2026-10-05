#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/RKIntegrator.h"
#include "QatGenericFunctions/CubicSplinePolynomial.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/Sin.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cmath>
#include <iostream>
#include <vector>

using namespace Genfun;

int main(int argc, char **argv) {

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

  // Constants
  double g = 9.8;   // gravity
  double l = 1.0;   // length
  double T_0 = 2.0 * M_PI * sqrt(l / g);  // small amplitude period

  // Amplitudes to test
  std::vector<double> amplitudes = {M_PI/12, M_PI/6, M_PI/3, M_PI/2,
                                     2*M_PI/3, 5*M_PI/6, 11*M_PI/12};

  CubicSplinePolynomial periodSpline;

  // For each amplitude
  for (double theta_max : amplitudes) {

    // Set up ODE: d^2*theta/dt^2 = -(g/l)sin(theta)
    // Rewrite as: d*theta/dt = v, dv/dt = -(g/l)sin(theta)
    Variable theta(0, 2), v(1, 2);
    Sin sin;

    // v is d*theta/dt
    GENFUNCTION eq1 = v;

    // dv/dt = -(g/l)sin(theta)
    GENFUNCTION eq2 = -(g/l) * sin(theta);

    // Create RK integrator
    RKIntegrator integrator;
    Parameter *thetaParam = integrator.addDiffEquation(&eq1, "theta", theta_max, 0, M_PI);
    Parameter *vParam = integrator.addDiffEquation(&eq2, "v", 0, -1, 1);

    // Get the solution fs
    const RKIntegrator::RKFunction *thetaSol = integrator.getFunction(0);
    const RKIntegrator::RKFunction *vSol = integrator.getFunction(1);

    // Find quarter-period. time when theta goes from theta_max to 0
    double t_quarter = 0;
    double t_step = 0.01;
    double t_max = 5.0;  // max time to search

    for (double t = 0; t < t_max; t += t_step) {
      Argument arg(2);
      arg[0] = t;
      arg[1] = 0;  // Doesn't get used

      double theta_t = (*thetaSol)(arg);

      // Crossed 0?
      if (theta_t < 0 || t > t_max) {
        t_quarter = t;
        break;
      }
    }

    // Full period
    double T = 4.0 * t_quarter;

    double f = T / T_0;
    
    // Add to spline (convert to degrees for x-axis)
    periodSpline.addPoint(theta_max * 180 / M_PI, f);
  }

  PRectF rect;
  rect.setXmin(0);
  rect.setXmax(180);
  rect.setYmin(0.9);
  rect.setYmax(1.2);

  PlotView view(rect);

  // Create smooth curve from spline
  PlotFunction1D periodPlot(periodSpline);

  PlotFunction1D::Properties props;
  props.pen.setColor(Qt::blue);
  props.pen.setWidth(3);
  periodPlot.setProperties(props);

  view.add(&periodPlot);
  window.setCentralWidget(&view);

  // Labels
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
              << PlotStream::Center()
              << PlotStream::Family("Arial")
              << PlotStream::Size(14)
              << "Pendulum Period Correction Factor"
              << PlotStream::EndP();

  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
               << PlotStream::Center()
               << PlotStream::Family("Arial")
               << PlotStream::Size(12)
               << "Amplitude theta_max (degrees)"
               << PlotStream::EndP();

  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
               << PlotStream::Center()
               << PlotStream::Family("Arial")
               << PlotStream::Size(12)
               << "f(theta_max) = T / T_0"
               << PlotStream::EndP();

  window.show();
  app.exec();
  return 0;
}
