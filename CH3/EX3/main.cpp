#include "decay.h"
#include <iostream>
#include <iomanip>

using namespace Eigen;
using namespace std;

int main()
{
    // Half-lives in minutes
    double hl_Bi = 45.6;      // Bi-213
    double hl_Tl = 132.0 / 60.0; // seconds to minutes
    double hl_Pb = 195.0;     // Pb-209

    // Decay matrix
    Matrix3d Lambda = buildDecayMatrix(hl_Bi, hl_Tl, hl_Pb);

    // Initial condition: N0 Bi atoms, 0 Tl, 0 Pb (normalize to 1)
    Vector3d x0(1.0, 0.0, 0.0);

    // Times to evaluate (in minutes)
    vector<double> times = {50, 150, 200, 500, 1000};

    cout << "Time (min) | Bi fraction | Tl fraction | Pb fraction" << endl;

    for (double t : times)
    {
        Vector3d pop = computePopulation(Lambda, x0, t);
        double total = pop(0) + pop(1) + pop(2);

        cout << fixed << setprecision(1) << t << " | ";
        cout << setprecision(6) << pop(0)/total << " | ";
        cout << pop(1)/total << " | ";
        cout << pop(2)/total << endl;
    }

    return 0;
}
