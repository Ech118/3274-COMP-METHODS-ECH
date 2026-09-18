#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/F1D.h"
#include "QatGenericFunctions/Sin.h"
#include "QatGenericFunctions/Exp.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>

using namespace Genfun;

// sin(x)
double Sinx(double x)
{
  return std::sin(x);
}

// sin(5x)
double Sin5x(double x)
{
  return std::sin(5*x);
}

// sin(x^2)
double Sinxx(double x)
{
  return std::sin(x*x);
}

// exp(-x)*sin(x)
double DecaySinx(double x)
{
  return std::exp(-x) * std::sin(x);
}

// associated Legendre function P_3^2(x), defined on [-1,1]
double Legendre(double x)
{
  return (15.0 * x * (1.0 - x*x));
}

// x^x
double Mine(double x)
{
  return std::pow(x, x);
}


int main (int argc, char * * argv)
{

  // Here are the inputs
  std::string usage= std::string("usage: ") + argv[0] + " <1-7>\n"
    "  1: sin(x)   2: sin(5x)   3: sin(x^2)   4: exp(-x)sin(x)\n"
    "  5: P_3^2(x)   6: x^x   7: d/dx x^x";
  if (argc != 2)
  {
    std::cout << usage << std::endl;
    return 1;
  }
  int choice = 0;
  try
  {
    choice = std::stoi(argv[1]);
  }
  catch (...)
  {
    choice = 0;
  }
  if (choice < 1 || choice > 7)
  {
    std::cout << usage << std::endl;
    return 1;
  }

  QApplication     app(argc,argv);
  
  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");
  
  quitAction->setShortcut(QKeySequence("q"));
  
  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);

  // Scale of Axis and function depending on the choice
  PRectF rect;
  double (*f)(double) = Sinx;
  std::string title;
  switch (choice)
  {
  case 1: f = Sinx;
          title = "sin(x)";
          rect.setXmin(0);
          rect.setXmax(10);
          rect.setYmin(-2);
          rect.setYmax(2);
          break;
  case 2: f = Sin5x;
          title = "sin(5x)";
          rect.setXmin(0);
          rect.setXmax(10);
          rect.setYmin(-2);
          rect.setYmax(2);
          break;
  case 3: f = Sinxx;
          title = "sin(x^2)";
          rect.setXmin(0);
          rect.setXmax(10);
          rect.setYmin(-2);
          rect.setYmax(2);
          break;
  case 4: f = DecaySinx;
          title = "exp(-x) sin(x)";
          rect.setXmin(0);
          rect.setXmax(10);
          rect.setYmin(-0.5);
          rect.setYmax(1);
          break;
  case 5: f = Legendre;
          title = "P_3^2(x)";
          rect.setXmin(-1);
          rect.setXmax(1);
          rect.setYmin(-6);
          rect.setYmax(6);
          break;
  case 6: f = Mine;
          title = "x^x";
          rect.setXmin(0);
          rect.setXmax(3);
          rect.setYmin(0);
          rect.setYmax(30);
          break;
  case 7: f = Mine;
          title = "d/dx x^x";
          rect.setXmin(0);
          rect.setXmax(3);
          rect.setYmin(-1);
          rect.setYmax(30);
          break;
  }
  
  // Set up Axis
  PlotView view(rect);
  window.setCentralWidget(&view);

  // Function
  // Case 7 plots the derivative of Mine, taken with F1D::prime()
  F1D F(f);
  PlotFunction1D pFunc = (choice == 7) ? PlotFunction1D(F.prime())
                                       : PlotFunction1D(F);
  {
    PlotFunction1D::Properties prop;
    prop.pen.setWidth(3);
    pFunc.setProperties(prop);
  }
  view.add(&pFunc);

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

