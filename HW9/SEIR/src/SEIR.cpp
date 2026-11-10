#include "QatPlotWidgets/PlotView.h"
#include "QatPlotWidgets/setLightPalette.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/RKIntegrator.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <string>

int main (int argc, char * * argv) {

  QApplication     app(argc,argv);
  setLightPalette();

  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");

  quitAction->setShortcut(QKeySequence("q"));

  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);

  // Parameters (1/day)
  const double beta  = 0.2;     // transmission rate
  const double gamma = 0.1;     // recovery rate, I -> R
  const double alpha = 0.1;     // incubation rate, E -> I
  const double b     = 1.0e-4;  // birth rate
  const double d     = 1.0e-4;  // death rate (same in every group)
  const double nu    = 1.0e-3;  // loss of immunity, R -> S

  const double N0   = 1.0;
  const double I0   = 1.0e-6;
  const double S0   = N0 - I0;
  const double tmax = 5000.0;   // days

  // Four variables: S, E, I, R
  Genfun::Variable S(0,4), E(1,4), I(2,4), R(3,4);

  Genfun::GENFUNCTION N       = S + E + I + R;
  Genfun::GENFUNCTION lambdaS = beta * I * S / N;   // force of infection times S

  Genfun::GENFUNCTION dSdt = b*N - lambdaS - d*S + nu*R;
  Genfun::GENFUNCTION dEdt = lambdaS - alpha*E - d*E;
  Genfun::GENFUNCTION dIdt = alpha*E - gamma*I - d*I;
  Genfun::GENFUNCTION dRdt = gamma*I - nu*R - d*R;

  Genfun::RKIntegrator integrator;
  integrator.addDiffEquation(&dSdt, "S0", S0);
  integrator.addDiffEquation(&dEdt, "E0", 0.0);
  integrator.addDiffEquation(&dIdt, "I0", I0);
  integrator.addDiffEquation(&dRdt, "R0", 0.0);

  Genfun::GENFUNCTION St = *integrator.getFunction(S);
  Genfun::GENFUNCTION Et = *integrator.getFunction(E);
  Genfun::GENFUNCTION It = *integrator.getFunction(I);
  Genfun::GENFUNCTION Rt = *integrator.getFunction(R);

  Genfun::GENFUNCTION infectedRatio = It / (St + Et + It + Rt);

  // Plot I/N on a log scale
  PRectF rect;
  rect.setXmin(0.0);
  rect.setXmax(tmax);
  rect.setYmin(1.0e-7);
  rect.setYmax(1.0);

  PlotView view(rect);
  view.setLogY(true);
  window.setCentralWidget(&view);

  PlotFunction1D pRatio = infectedRatio;
  {
    PlotFunction1D::Properties prop;
    prop.pen.setWidth(2);
    pRatio.setProperties(prop);
  }
  view.add(&pRatio);

  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center()
	      << PlotStream::Family("Arial")
	      << PlotStream::Size(16)
	      << "SEIR: infected population ratio"
	      << PlotStream::EndP();

  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << "t (days)"
	       << PlotStream::EndP();

  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << "I/N"
	       << PlotStream::EndP();

  window.resize(900, 600);
  window.show();
  return app.exec();
}
