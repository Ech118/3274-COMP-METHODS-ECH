#include "schrodinger.h"
#include <iostream>

using namespace std;

int main()
{
    // Set barrier width
    double a = 1.0;

    // Scan ranges
    vector<double> V0_over_E = {0.5, 1.0, 2.0, 5.0};
    vector<double> ka_values = {0.5, 1.0, 2.0, 3.0, 5.0};

    cout << "V0/E | ka=0.5 | ka=1.0 | ka=2.0 | ka=3.0 | ka=5.0" << endl;
    cout << "----|--------|--------|--------|--------|--------" << endl;

    for (double ratio : V0_over_E)
    {
        cout << ratio << " | ";
        for (double ka : ka_values)
        {
            double E = ka / a;
            double V0 = ratio * E;

            TransmissionData data = solveSchrodinger(E, V0, a);
            cout << data.T << " | ";
        }
        cout << endl;
    }

    return 0;
}
