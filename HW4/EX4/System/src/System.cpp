#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotProfile.h"
#include "QatPlotting/PlotKey.h"
#include <Eigen/Dense>
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

using namespace Eigen;

const int    N = 100;  // number of beads
const double M = 1.0;  // mass of each bead
const double K = 1.0;  // spring constant (bead-bead and bead-wall)

int main (int argc, char * * argv)
{
  std::string usage = std::string("usage: ") + argv[0];
  if (argc != 1)
  {
    std::cout << usage << std::endl;
    return 1;
  }

  // Build the stiffness matrix for N beads on a string, each bead coupled
  // to its neighbors by a spring of constant K, with the first and last
  // beads additionally tied to fixed walls by a spring of constant K.
  MatrixXd stiffness = MatrixXd::Zero(N, N);
  for (int i = 0; i < N; i++)
  {
    stiffness(i, i) = 2.0 * K;
  }
  for (int i = 0; i < N - 1; i++)
  {
    stiffness(i, i + 1) = -K;
    stiffness(i + 1, i) = -K;
  }

  // m * x'' = -stiffness * x, with all masses equal, so the normal modes
  // are the eigenvectors of stiffness, and omega^2 = eigenvalue / M.
  SelfAdjointEigenSolver<MatrixXd> solver(stiffness);
  const VectorXd & eigenvalues  = solver.eigenvalues();
  const MatrixXd & eigenvectors = solver.eigenvectors();

  const int nModes = 5;
  std::cout << "Lowest " << nModes << " eigenfrequencies (omega, in units of sqrt(k/m)):" << std::endl;
  std::cout << std::setprecision(6);
  for (int p = 0; p < nModes; p++)
  {
    double omega = std::sqrt(eigenvalues(p) / M);
    std::cout << "  mode " << (p + 1) << ": omega = " << omega << std::endl;
  }

  QApplication     app(argc,argv);

  QMainWindow window;
  QToolBar *toolBar=window.addToolBar("Tools");
  QAction  *quitAction=toolBar->addAction("Quit");

  quitAction->setShortcut(QKeySequence("q"));

  QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);

  // Find the largest |component| among the modes we plot, to size the y-axis
  double yMax = 0.0;
  for (int p = 0; p < nModes; p++)
  {
    for (int i = 0; i < N; i++)
    {
      yMax = std::max(yMax, std::abs(eigenvectors(i, p)));
    }
  }
  yMax *= 1.2;

  PRectF rect;
  rect.setXmin(0.0);
  rect.setXmax(N + 1.0);
  rect.setYmin(-yMax);
  rect.setYmax(yMax);

  PlotView view(rect);
  window.setCentralWidget(&view);

  const std::vector<std::string> colors =
    {"darkRed", "darkBlue", "darkGreen", "darkMagenta", "darkCyan"};

  std::vector<std::unique_ptr<PlotProfile>> profiles;
  PlotKey key(0.7 * (N + 1.0), 0.9 * yMax);
  for (int p = 0; p < nModes; p++)
  {
    // Fix an overall sign convention so the first bead always moves positive
    double sign = (eigenvectors(0, p) < 0.0) ? -1.0 : 1.0;

    auto profile = std::make_unique<PlotProfile>();
    for (int i = 0; i < N; i++)
    {
      profile->addPoint(i + 1, sign * eigenvectors(i, p));
    }

    PlotProfile::Properties prop;
    prop.pen.setColor(QString::fromStdString(colors[p]));
    prop.brush.setColor(QString::fromStdString(colors[p]));
    prop.brush.setStyle(Qt::SolidPattern);
    prop.symbolStyle = PlotProfile::Properties::CIRCLE;
    prop.symbolSize = 4;
    prop.drawSymbol = true;
    profile->setProperties(prop);

    view.add(profile.get());
    key.add(profile.get(), "mode " + std::to_string(p + 1));
    profiles.push_back(std::move(profile));
  }
  view.add(&key);

  PlotStream titleStream(view.titleTextEdit());
  titleStream << PlotStream::Clear()
	      << PlotStream::Center()
	      << PlotStream::Family("Arial")
	      << PlotStream::Size(16)
	      << "First 5 eigenmodes of " << N << " beads on a string"
	      << PlotStream::EndP();

  PlotStream xLabelStream(view.xLabelTextEdit());
  xLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << "bead index i"
	       << PlotStream::EndP();

  PlotStream yLabelStream(view.yLabelTextEdit());
  yLabelStream << PlotStream::Clear()
	       << PlotStream::Center()
	       << PlotStream::Family("Arial")
	       << PlotStream::Size(16)
	       << "displacement of bead i"
	       << PlotStream::EndP();

  window.show();
  app.exec();
  return 0;
}
