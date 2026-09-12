#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <Eigen/Dense>
#include <vector>

using namespace Eigen;
using namespace std;

struct OscillatorSystem
{
    VectorXd masses;
    MatrixXd stiffness;
    VectorXd initial_positions;
    VectorXd initial_velocities;
    
    VectorXd eigenvalues;
    MatrixXd eigenvectors;
};

struct TimeEvolution
{
    vector<double> times;
    MatrixXd positions;  // each column is a mass, each row is a time step
};

// Build the coupled oscillator system
OscillatorSystem buildSystem();

// Find normal modes using eigenvalue decomposition
void findNormalModes(OscillatorSystem& sys);

// Evolve the system in time
TimeEvolution evolveSystem(const OscillatorSystem& sys, double t_max, int num_steps);

// Calculate center of mass position
VectorXd calculateCenterOfMass(const OscillatorSystem& sys, const MatrixXd& positions);

#endif
