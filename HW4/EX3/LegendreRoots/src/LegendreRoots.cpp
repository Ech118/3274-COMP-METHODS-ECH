#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatPlotting/PlotPoint.h"
#include "QatGenericFunctions/LegendrePolynomial.h"
#include "QatGenericFunctions/RootFinder.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <iomanip>

using namespace Genfun;

int main (int argc, char * * argv)
{
  std::string usage = std::string("usage: ") + argv[0] + " <l>\n"
    "  l: order of the Legendre polynomial, 1 through 10";
  if (argc != 2)
  {
    std::cout << usage << std::endl;
    return 1;
  }
  int l = 0;
  try
  {
    l = std::stoi(argv[1]);
  }
  catch (...)
  {
    l = 0;
  }
  if (l < 1 || l > 10)
  {
    std::cout << usage << std::endl;
    return 1;
  }

  LegendrePolynomial P(l);
  NewtonRaphson finder(P);

  // The m-th zero (m = 1..l) starts from the asymptotic expression
  std::vector<double> zeros;
  for (int m = 1; m <= l; m++)
  {
    double guess = (1.0 - 1.0/(8.0*l*l) + 1.0/(8.0*l*l*l))
                 * std::cos((4.0*m - 1.0) / (4.0*l + 2.0) * M_PI);
    zeros.push_back(finder.root(guess));
  }

  std::cout << "Zeros of P_" << l << "(x):" << std::endl;
  std::cout << std::setprecision(10);
  for (double z : zeros)
  {
    std::cout << "  " << z << std::endl;
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
  rect.setYmin(-1.2);
  rect.setYmax(1.2);
  

  PlotView view(rect);
  window.setCentralWidget(&view);

  // The polynomial
  PlotFunction1D pPoly = P;
  {
    PlotFunction1D::Properties prop;
    prop.pen.setWidth(3);
    pPoly.setProperties(prop);
  }
  view.add(&pPoly);

  // The zeros, marked on the x-axis
  std::vector<std::unique_ptr<PlotPoint>> points;
  for (double z : zeros)
  {
    points.push_back(std::make_unique<PlotPoint>(z, 0.0));
    PlotPoint::Properties prop;
    prop.symbolStyle = PlotPoint::Properties::CIRCLE;
    prop.symbolSize = 8;
    prop.pen.setColor("darkRed");
    prop.brush.setColor("red");
    prop.brush.setStyle(Qt::SolidPattern);
    points.back()->setProperties(prop);
    view.add(points.back().get());
  }
  
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center() 
	      << PlotStream::Family("Arial") 
	      << PlotStream::Size(16)
	      << "Zeros of P" << PlotStream::Sub() << l << PlotStream::Normal() << "(x)"
	      << PlotStream::EndP();
  
  
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << "x"
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << PlotStream::EndP();
  
  
  
  window.show();
  app.exec();
  return 0;
}

