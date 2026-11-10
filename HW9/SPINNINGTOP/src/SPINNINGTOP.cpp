#include "QatPlotWidgets/PlotView.h"
#include "QatGenericFunctions/PhaseSpace.h"
#include "QatGenericFunctions/RungeKuttaClassicalSolver.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatPlotWidgets/setLightPalette.h"
#include "QatPlotting/PlotStream.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QWidget>
#include <QGridLayout> 
#include <cstdlib>
#include <iostream>
#include <string>

// Writes title, x label and y label onto one PlotView
static void labelView(PlotView &view, const std::string &title, const std::string &ylabel) {
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
               << "Time"
               << PlotStream::EndP();

  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
               << PlotStream::Center()
               << PlotStream::Family("Arial")
               << PlotStream::Size(16)
               << ylabel
               << PlotStream::EndP();
}

int main (int argc, char * * argv) {

  // Automatically generated:-------------------------:

  std::string usage= std::string("usage: ") + argv[0];
  if (argc!=1) {
    std::cout << usage << std::endl;
  }

  QApplication     app(argc,argv);
  setLightPalette();

  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");

  quitAction->setShortcut(QKeySequence("q"));

  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);

  // Params
  double I1 = 1.0;
  double I3 = 0.5;
  double Mgl = 1.0;

  Classical::PhaseSpace phaseSpace(3);           // u, phi, psi
  const Classical::PhaseSpace::Component
    &q = phaseSpace.coordinates(),
    &p = phaseSpace.momenta();

  // Initial conditions: theta0 = 60 deg, released with phidot = 0, udot = 0
  double u0 = 0.5, omega3 = 10.0;
  double ppsi = I3 * omega3;
  double pphi = u0 * ppsi;

  phaseSpace.start(q[0], u0);
  phaseSpace.start(q[1], 0.0);
  phaseSpace.start(q[2], 0.0);
  phaseSpace.start(p[0], 0.0);
  phaseSpace.start(p[1], pphi);
  phaseSpace.start(p[2], ppsi);

  Genfun::GENFUNCTION H = (1.0 - q[0]*q[0]) * p[0]*p[0] / (2.0*I1)
  + (p[1] - q[0]*p[2]) * (p[1] - q[0]*p[2]) / (2.0*I1*(1.0 - q[0]*q[0]))
  + p[2]*p[2] / (2.0*I3)
  + Mgl * q[0];

  Classical::RungeKuttaSolver solver(H, phaseSpace);
  Genfun::GENFUNCTION u   = solver.equationOf(q[0]);
  Genfun::GENFUNCTION phi = solver.equationOf(q[1]);
  Genfun::GENFUNCTION psi = solver.equationOf(q[2]);

  // PlotView per variable, each with its own y-range
  PRectF rU;
  rU.setXmin(0.0);  rU.setXmax(10.0);
  rU.setYmin(-1.0); rU.setYmax(1.0);

  PRectF rPhi;
  rPhi.setXmin(0.0); rPhi.setXmax(10.0);
  rPhi.setYmin(0.0); rPhi.setYmax(3.0);

  PRectF rPsi;
  rPsi.setXmin(0.0); rPsi.setXmax(10.0);
  rPsi.setYmin(0.0); rPsi.setYmax(110.0);

  PlotView viewU(rU), viewPhi(rPhi), viewPsi(rPsi);

  PlotFunction1D pU = u, pPhi = phi, pPsi = psi;
  viewU.add(&pU);
  viewPhi.add(&pPhi);
  viewPsi.add(&pPsi);

  labelView(viewU,   "Nutation",   "u = cos(theta)");
  labelView(viewPhi, "Precession", "phi");
  labelView(viewPsi, "Spin",       "psi");

  QWidget *central = new QWidget;
  QGridLayout *layout = new QGridLayout(central);
  layout->addWidget(&viewU,   0, 0, 1, 2);   // row 0, col 0, spans 1 row, spans 2 columns
  layout->addWidget(&viewPhi, 1, 0);         // row 1, left
  layout->addWidget(&viewPsi, 1, 1);         // row 1, right
  window.setCentralWidget(central);
  window.resize(900, 600);

  window.show();
  return app.exec();
}