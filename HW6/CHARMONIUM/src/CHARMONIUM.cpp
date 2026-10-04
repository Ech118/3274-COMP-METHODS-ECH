#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatPlotting/PlotText.h"
#include "QatPlotting/RealArg.h"
#include "QatGenericFunctions/FixedConstant.h"
#include "QatGenericFunctions/GaussQuadratureRule.h"
#include "QatGenericFunctions/RootFinder.h"
#include "QatGenericFunctions/Variable.h"
#include "QatGenericFunctions/F1D.h"
#include "QatGenericFunctions/GaussIntegrator.h"
#include "QatGenericFunctions/Sin.h"
#include "QatGenericFunctions/Sqrt.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// Units: MeV and fm
const double hbarc = 197.327;
const double mc    = 1370.0;
const double mu    = mc / 2.0;              // reduced mass
const double alpha = 0.38;
const double a     = 2.43e-3;               // MeV^-1
const double A     = 4.0/3.0*alpha*hbarc;
const double k     = 1.0/(hbarc*a*a);
const double rTiny = 1e-8, rBig = 100.0;

Genfun::Variable R;

// Langer-corrected centrifugal strength, l(l+1) -> (l+1/2)^2
double B(int l) { return hbarc*hbarc*(l+0.5)*(l+0.5)/(2.0*mu); }

double rMinimum(int l) {
  Genfun::GENFUNCTION Veff  = -A/R + k*R + B(l)/(R*R);
  Genfun::GENFUNCTION dVeff = Veff.prime();     // Bisection holds a reference, so no temporaries
  Genfun::Bisection finder(dVeff);
  finder.lowerBound() = rTiny;
  finder.upperBound() = rBig;
  return finder.root(1.0);
}

// (1/hbar) * integral of p dr between the turning points
double phaseIntegral(double E, int l) {
  Genfun::GENFUNCTION Veff = -A/R + k*R + B(l)/(R*R);

  double rMin = rMinimum(l);
  Genfun::GENFUNCTION g = Veff - E;
  Genfun::Bisection finder(g);
  finder.lowerBound() = rTiny;  finder.upperBound() = rMin;
  double r1 = finder.root(0.5*rMin);
  finder.lowerBound() = rMin;   finder.upperBound() = rBig;
  double r2 = finder.root(2.0*rMin);

  // r = r1 + (r2-r1) sin^2(theta) removes the square-root behaviour at the turning points
  static Genfun::GaussLegendreRule rule(64);
  static Genfun::GaussIntegrator   integrator(rule);
  Genfun::Variable U;
  Genfun::Sin  sin;
  Genfun::Sqrt sqrt;
  double c = (M_PI/2)/(rule.max() - rule.min());
  Genfun::GENFUNCTION theta = c*(U - rule.min());
  Genfun::GENFUNCTION r     = r1 + (r2 - r1)*sin(theta)*sin(theta);
  Genfun::GENFUNCTION drdU  = (r2 - r1)*c*sin(2*theta);
  Genfun::GENFUNCTION p     = sqrt(2.0*mu*(E - Veff(r)));   // p c, in MeV

  return integrator(p*drdU) / hbarc;
}

// WKB rule: integral of p dr = (n - 1/2) pi hbar.
// F1D wraps a plain function pointer, so l and the target are passed through globals.
int    currentL;
double currentTarget;
double quantization(double E) { return phaseIntegral(E, currentL) - currentTarget; }

double energy(int n, int l) {
  currentL      = l;
  currentTarget = (n - 0.5) * M_PI;

  Genfun::GENFUNCTION Veff = -A/R + k*R + B(l)/(R*R);
  Genfun::F1D condition(quantization);
  Genfun::Bisection finder(condition);
  finder.lowerBound() = Veff(rMinimum(l)) + 1e-6;   // bottom of the well
  finder.upperBound() = 3000.0;
  return finder.root(500.0);
}

int main (int argc, char * * argv) {

  // Automatically generated:-------------------------:

  std::string usage= std::string("usage: ") + argv[0];
  if (argc!=1) {
    std::cout << usage << std::endl;
  }

  // a), b), c): energies and masses of the three lowest S and P states
  double mass[2][3];
  std::cout << std::fixed << std::setprecision(1);
  std::cout << "state   E (MeV)   M = 2 m_c + E (MeV)" << std::endl;
  for (int l = 0; l <= 1; l++) {
    for (int n = 1; n <= 3; n++) {
      double E = energy(n, l);
      mass[l][n-1] = 2.0*mc + E;
      std::cout << n << (l == 0 ? "S" : "P") << "    " << std::setw(8) << E
                << "     " << std::setw(8) << mass[l][n-1] << std::endl;
    }
  }


  QApplication     app(argc,argv);

  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");

  quitAction->setShortcut(QKeySequence("q"));

  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);

  PRectF rect;
  rect.setXmin(0.0);
  rect.setXmax(3.5);
  rect.setYmin(2900.0);
  rect.setYmax(4450.0);


  PlotView view(rect);
  window.setCentralWidget(&view);

  // c) Energy level diagram: S levels in the left column, P levels in the right
  std::vector<Genfun::FixedConstant> levelFunc;
  std::vector<PlotFunction1D>        levelPlot;
  std::vector<PlotText>              levelText;
  levelFunc.reserve(6); levelPlot.reserve(6); levelText.reserve(8);

  PlotFunction1D::Properties prop;
  prop.pen.setWidth(3);

  for (int l = 0; l <= 1; l++) {
    double x0 = (l == 0 ? 0.5 : 2.0), x1 = x0 + 1.0;
    prop.pen.setColor(l == 0 ? "blue" : "red");
    for (int n = 1; n <= 3; n++) {
      levelFunc.emplace_back(mass[l][n-1]);
      levelPlot.emplace_back(levelFunc.back(), RealArg::Gt(x0) && RealArg::Lt(x1));
      levelPlot.back().setProperties(prop);
      view.add(&levelPlot.back());

      std::string label = std::to_string(n) + (l == 0 ? "S" : "P");
      levelText.emplace_back(x1 + 0.05, mass[l][n-1] + 25, QString::fromStdString(label));
      view.add(&levelText.back());
    }
  }
  levelText.emplace_back(0.75, 2990, QString("S (l = 0)"));
  view.add(&levelText.back());
  levelText.emplace_back(2.25, 2990, QString("P (l = 1)"));
  view.add(&levelText.back());

  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center()
	      << PlotStream::Family("Arial")
	      << PlotStream::Size(16)
	      << "Charmonium energy levels (WKB)"
	      << PlotStream::EndP();


  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << PlotStream::EndP();

  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << "Mass (MeV/c²)"
	       << PlotStream::EndP();



  window.show();
  app.exec();
  return 0;
}
