#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatDataAnalysis/Table.h"
#include "QatPlotting/PlotProfile.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/CubicSplinePolynomial.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <cstdlib>
#include <iostream>
#include <string>

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
  rect.setXmin(200.0);
  rect.setXmax(1000.0);
  rect.setYmin(1.327);
  rect.setYmax(1.396);
  

  PlotView view(rect);
  window.setCentralWidget(&view);
    
  Table hale("Dispersion of Light");

  double ref[] = {1.396, 1.362, 1.349, 1.343, 1.339, 1.337, 1.335, 
                  1.333, 1.332, 1.331, 1.331, 1.330, 1.329, 1.329, 
                  1.328, 1.327, 1.327};

  int wavelengths[] = {200, 250, 300, 350, 400, 450, 500, 550, 600, 
                      650, 700, 750, 800, 850, 900, 950, 1000};

  // Create table
  for(int i = 0; i < 17; i++) {
      hale.add("wavelength (nm)", wavelengths[i]);
      hale.add("Index of refraction", ref[i]);
      hale.capture();
  }

  PlotProfile plot;

  for (size_t i = 0; i < hale.numTuples(); i++) {
    auto tuple = hale[i];
    
    int wl;
    double n;
    
    tuple->read(n, 0);   // Read second column
    tuple->read(wl, 1);  // Read first column

    // Testing
    std::cout << "Col1: " << wl << std::endl;
    std::cout << "Col2: " << n << std::endl;
    
    plot.addPoint(wl, n);
  }

  CubicSplinePolynomial spline;

  for (size_t i = 0; i < hale.numTuples(); i++) {
    auto tuple = hale[i];
    int wl;
    double n;
    
    tuple->read(n, 0);
    tuple->read(wl, 1);
    
    spline.addPoint(wl, n);  // Add (wavelength, index) pair
  }

  PlotFunction1D interp(spline);
  view.add(&interp);
  view.add(&plot);

  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center() 
	      << PlotStream::Family("Arial") 
	      << PlotStream::Size(16)
        << "Dispersion of Light"
	      << PlotStream::EndP();
  
  
  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
         << "Wavelength (nm)"
	       << PlotStream::EndP();
  
  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
         << "Index of refraction"
	       << PlotStream::EndP();
  
  
  
  window.show();
  app.exec();
  return 0;
}

