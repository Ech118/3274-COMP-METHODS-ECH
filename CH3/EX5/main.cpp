#include "oscillator.h"
#include "QatPlotWidgets/PlotView.h"
#include "QatPlotting/PlotStream.h"
#include "QatPlotting/PlotFunction1D.h"
#include "QatGenericFunctions/F1D.h"
#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <iostream>

using namespace std;

// Global data for plotting
TimeEvolution g_evolution;
VectorXd g_com;
OscillatorSystem g_sys;

double interpolate(double t, const vector<double>& times, const VectorXd& values)
{
    int n = times.size();
    if (t <= times[0]) return values(0);
    if (t >= times[n-1]) return values(n-1);

    int idx = 0;
    for (int i = 0; i < n - 1; i++)
    {
        if (t >= times[i] && t < times[i+1])
        {
            idx = i;
            break;
        }
    }

    double t0 = times[idx];
    double t1 = times[idx + 1];
    double x0 = values(idx);
    double x1 = values(idx + 1);

    double frac = (t - t0) / (t1 - t0);
    return x0 + frac * (x1 - x0);
}

// Mass position functions
double mass1(double t)
{
    return interpolate(t, g_evolution.times, g_evolution.positions.col(0));
}

double mass2(double t)
{
    return interpolate(t, g_evolution.times, g_evolution.positions.col(1));
}

double mass3(double t)
{
    return interpolate(t, g_evolution.times, g_evolution.positions.col(2));
}

double mass4(double t)
{
    return interpolate(t, g_evolution.times, g_evolution.positions.col(3));
}

double mass5(double t)
{
    return interpolate(t, g_evolution.times, g_evolution.positions.col(4));
}

double centerOfMass(double t)
{
    return interpolate(t, g_evolution.times, g_com);
}

int main(int argc, char* argv[])
{
    // Build system
    g_sys = buildSystem();

    cout << "Masses: " << g_sys.masses.transpose() << endl;
    cout << "Initial positions: " << g_sys.initial_positions.transpose() << endl;

    // Find the normal modes
    findNormalModes(g_sys);

    cout << "Eigenfrequencies (omega^2): " << g_sys.eigenvalues.transpose() << endl;

    // Evolve the system
    double t_max = 20.0;
    int num_steps = 1000;
    g_evolution = evolveSystem(g_sys, t_max, num_steps);

    // Calculate center of mass
    g_com = calculateCenterOfMass(g_sys, g_evolution.positions);

    // Qt application setup
    QApplication app(argc, argv);
    QMainWindow window;
    QToolBar* toolBar = window.addToolBar("Tools");
    QAction* quitAction = toolBar->addAction("Quit");
    quitAction->setShortcut(QKeySequence("q"));
    QObject::connect(quitAction, &QAction::triggered, &app, &QApplication::quit);

    // Set up plot view
    PRectF rect;
    rect.setXmin(0.0);
    rect.setXmax(t_max);
    rect.setYmin(-4.0);
    rect.setYmax(4.0);

    PlotView view(rect);
    window.setCentralWidget(&view);

    // Plot center of mass
    PlotFunction1D pCOM = Genfun::F1D(centerOfMass);
    {
        PlotFunction1D::Properties prop;
        prop.pen.setWidth(3);
        pCOM.setProperties(prop);
    }
    view.add(&pCOM);

    // Title
    PlotStream titleStream(view.titleTextEdit());
    titleStream << PlotStream::Clear()
                << PlotStream::Center()
                << PlotStream::Family("Sans Serif")
                << PlotStream::Size(16)
                << "Center of Mass Position vs Time"
                << PlotStream::EndP();

    // X-axis label
    PlotStream xLabelStream(view.xLabelTextEdit());
    xLabelStream << PlotStream::Clear()
                 << PlotStream::Center()
                 << PlotStream::Family("Sans Serif")
                 << PlotStream::Size(14)
                 << "time (sqrt(m/k))"
                 << PlotStream::EndP();

    // Y-axis label
    PlotStream yLabelStream(view.yLabelTextEdit());
    yLabelStream << PlotStream::Clear()
                 << PlotStream::Center()
                 << PlotStream::Family("Sans Serif")
                 << PlotStream::Size(14)
                 << "position (cm)"
                 << PlotStream::EndP();

    window.show();
    return app.exec();
}
