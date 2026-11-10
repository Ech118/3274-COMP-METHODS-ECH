

#include "SymplecticIntegrator.h"
#include "QatGenericFunctions/PhaseSpace.h"
#include "QatGenericFunctions/RungeKuttaClassicalSolver.h"
#include "QatGenericFunctions/SimpleRKStepper.h"
#include "QatGenericFunctions/ButcherTableau.h"
#include "QatDataAnalysis/OptParse.h"
#include "QatPlotWidgets/PlotView.h"
#include "QatPlotWidgets/setLightPalette.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotProfile.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <algorithm>
#include <string>

static void labelView(PlotView &view, const std::string &title,
                      const std::string &xlabel, const std::string &ylabel) {
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear() << PlotStream::Center()
              << PlotStream::Family("Arial") << PlotStream::Size(16)
              << title << PlotStream::EndP();
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear() << PlotStream::Center()
               << PlotStream::Family("Arial") << PlotStream::Size(16)
               << xlabel << PlotStream::EndP();
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear() << PlotStream::Center()
               << PlotStream::Family("Arial") << PlotStream::Size(16)
               << ylabel << PlotStream::EndP();
}

int main (int argc, char * * argv) {

  QApplication app(argc, argv);
  setLightPalette();

  NumericInput input;
  input.declare("h",    "step size", 0.3);
  input.declare("tmax", "end time",  10000.0);
  input.optParse(argc, argv);
  const double h    = input.getByName("h");
  const double tmax = input.getByName("tmax");

  // Harmonic oscillator, starting at q = 1, p = 0
  Classical::PhaseSpace phaseSpace(1);
  const Classical::PhaseSpace::Component &q = phaseSpace.coordinates(), &p = phaseSpace.momenta();
  phaseSpace.start(q[0], 1.0);
  phaseSpace.start(p[0], 0.0);

  Genfun::GENFUNCTION H = 0.5 * (p[0]*p[0] + q[0]*q[0]);

  // The two integrators, same step size
  SymplecticIntegrator leap(H, phaseSpace, h);

  Genfun::SimpleRKStepper rk4Stepper(Genfun::ClassicalRungeKuttaTableau(), h);
  Classical::RungeKuttaSolver rk4(H, phaseSpace, &rk4Stepper);
  Genfun::GENFUNCTION Erk4 = rk4.energy();

  // Sample E - E0 for both, tracking the y-range for the plot
  PlotProfile profLeap, profRk4;
  const double E0 = leap.energy();
  const int nSamples = 1000;
  double lo = 0, hi = 0;

  for (int k = 1; k <= nSamples; k++) {
    leap.integrate(tmax * k / nSamples);
    double t  = leap.time();
    double dl = leap.energy() - E0;
    double dr = Erk4(t) - E0;
    profLeap.addPoint(t, dl);
    profRk4.addPoint(t, dr);
    lo = std::min({lo, dl, dr});
    hi = std::max({hi, dl, dr});
  }

  // Plot
  {
    PlotProfile::Properties prop;
    prop.symbolSize = 2;
    prop.pen.setColor(Qt::blue);  prop.brush = QBrush(Qt::blue);
    profLeap.setProperties(prop);
    prop.pen.setColor(Qt::red);   prop.brush = QBrush(Qt::red);
    profRk4.setProperties(prop);
  }

  double pad = 0.1 * (hi - lo + 1e-15);
  PRectF rect;
  rect.setXmin(0.0);       rect.setXmax(tmax);
  rect.setYmin(lo - pad);  rect.setYmax(hi + pad);

  PlotView view(rect);
  view.add(&profLeap);
  view.add(&profRk4);
  labelView(view, "Oscillator: leapfrog (blue) vs RK4 (red)", "t", "E - E0");

  QMainWindow window;
  QToolBar *toolBar = window.addToolBar("Tools");
  QAction  *quitAction = toolBar->addAction("Quit");
  quitAction->setShortcut(QKeySequence("q"));
  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);

  window.setCentralWidget(&view);
  window.resize(900, 600);
  window.show();
  return app.exec();
}
