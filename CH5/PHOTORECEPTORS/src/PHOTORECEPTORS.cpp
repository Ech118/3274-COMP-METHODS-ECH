#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/CubicSplinePolynomial.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <iostream>

using namespace Genfun;

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
  rect.setXmin(380.0);
  rect.setXmax(680.0);
  rect.setYmin(0.0);
  rect.setYmax(105.0);

  PlotView view(rect);
  window.setCentralWidget(&view);

  // DATA

  // Blue Cones data (peak 420nm)
  double blueLambda[] = {380, 400, 410, 420, 430, 440, 460, 480, 500, 520};
  double blueAbs[] = {58, 62, 75, 100, 98, 92, 78, 50, 25, 8};
  CubicSplinePolynomial blueCones;
  for (int i = 0; i < 10; i++) {
    blueCones.addPoint(blueLambda[i], blueAbs[i]);
  }

  // Rods data (peak 498nm)
  double rodsLambda[] = {380, 400, 410, 420, 430, 440, 460, 480, 498, 520, 540, 560, 580, 600};
  double rodsAbs[] = {38, 38, 40, 42, 44, 48, 60, 80, 100, 85, 55, 30, 12, 3};
  CubicSplinePolynomial rods;
  for (int i = 0; i < 14; i++) {
    rods.addPoint(rodsLambda[i], rodsAbs[i]);
  }

  // Green Cones data (peak 534nm)
  double greenLambda[] = {380, 400, 410, 420, 430, 440, 460, 480, 500, 520, 534, 550, 570, 590, 610, 630};
  double greenAbs[] = {8, 8, 10, 12, 15, 20, 32, 50, 68, 85, 100, 95, 65, 30, 10, 2};
  CubicSplinePolynomial greenCones;
  for (int i = 0; i < 16; i++) {
    greenCones.addPoint(greenLambda[i], greenAbs[i]);
  }

  // Red Cones data (peak 564nm)
  double redLambda[] = {380, 400, 410, 420, 430, 440, 460, 480, 500, 520, 540, 560, 564, 580, 600, 620, 640, 660};
  double redAbs[] = {8, 8, 10, 12, 14, 18, 28, 45, 65, 82, 95, 98, 100, 98, 80, 50, 25, 8};
  CubicSplinePolynomial redCones;
  for (int i = 0; i < 18; i++) {
    redCones.addPoint(redLambda[i], redAbs[i]);
  }

  // Promote
  PlotFunction1D bluePlot(blueCones);
  PlotFunction1D rodsPlot(rods);
  PlotFunction1D greenPlot(greenCones);
  PlotFunction1D redPlot(redCones);

  // Style
  PlotFunction1D::Properties blueProps, rodsProps, greenProps, redProps;

  blueProps.pen.setColor(Qt::blue);
  blueProps.pen.setWidth(2);
  bluePlot.setProperties(blueProps);

  rodsProps.pen.setColor(Qt::black);
  rodsProps.pen.setWidth(2);
  rodsPlot.setProperties(rodsProps);

  greenProps.pen.setColor(Qt::green);
  greenProps.pen.setWidth(2);
  greenPlot.setProperties(greenProps);

  redProps.pen.setColor(Qt::red);
  redProps.pen.setWidth(2);
  redPlot.setProperties(redProps);

  // Add to view
  view.add(&bluePlot);
  view.add(&rodsPlot);
  view.add(&greenPlot);
  view.add(&redPlot);

  // title and labels
  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
              << PlotStream::Center()
              << PlotStream::Family("Arial")
              << PlotStream::Size(16)
              << "Human Photoreceptor Spectral Response (Bowmaker, 1980)"
              << PlotStream::EndP();

  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
               << PlotStream::Center()
               << PlotStream::Family("Arial")
               << PlotStream::Size(14)
               << "Wavelength (nm)"
               << PlotStream::EndP();

  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
               << PlotStream::Center()
               << PlotStream::Family("Arial")
               << PlotStream::Size(14)
               << "Normalized Absorbance"
               << PlotStream::EndP();

  window.show();
  app.exec();
  return 0;
}
