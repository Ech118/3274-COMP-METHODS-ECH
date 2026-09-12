#include "oscillator.h"
#include <cmath>
#include <iostream>

OscillatorSystem buildSystem()
{
    OscillatorSystem sys;

    // Masses m, 3m, 2m, m, 2m (I did m = 1 for simplicity)
    sys.masses = VectorXd(5);
    sys.masses << 1.0, 3.0, 2.0, 1.0, 2.0;

    // Spring constants k, 2k, 2k, k (k = 1)
    // Build stiffness matrix (5x5)
    sys.stiffness = MatrixXd::Zero(5, 5);

    // Spring 1 (k) between masses 0 and 1
    sys.stiffness(0, 0) += 1.0;
    sys.stiffness(1, 1) += 1.0;
    sys.stiffness(0, 1) -= 1.0;
    sys.stiffness(1, 0) -= 1.0;

    // Spring 2 (2k) between masses 1 and 2
    sys.stiffness(1, 1) += 2.0;
    sys.stiffness(2, 2) += 2.0;
    sys.stiffness(1, 2) -= 2.0;
    sys.stiffness(2, 1) -= 2.0;

    // Spring 3 (2k) between masses 2 and 3
    sys.stiffness(2, 2) += 2.0;
    sys.stiffness(3, 3) += 2.0;
    sys.stiffness(2, 3) -= 2.0;
    sys.stiffness(3, 2) -= 2.0;

    // Spring 4 (k) between masses 3 and 4
    sys.stiffness(3, 3) += 1.0;
    sys.stiffness(4, 4) += 1.0;
    sys.stiffness(3, 4) -= 1.0;
    sys.stiffness(4, 3) -= 1.0;

    // Initial positions -2, 3, 0, -3, 2 cm
    sys.initial_positions = VectorXd(5);
    sys.initial_positions << -2.0, 3.0, 0.0, -3.0, 2.0;

    // Initial velocities are 0
    sys.initial_velocities = VectorXd::Zero(5);

    return sys;
}

void findNormalModes(OscillatorSystem& sys)
{
    // Build M^(-1/2) K M^(-1/2)
    MatrixXd M_inv_sqrt = MatrixXd::Zero(5, 5);
    for (int i = 0; i < 5; i++)
    {
        M_inv_sqrt(i, i) = 1.0 / sqrt(sys.masses(i));
    }

    MatrixXd A = M_inv_sqrt * sys.stiffness * M_inv_sqrt;

    // Solve
    SelfAdjointEigenSolver<MatrixXd> solver(A);

    VectorXd eigenvalues_sqrt = solver.eigenvalues();
    MatrixXd eigenvectors_sqrt = solver.eigenvectors();

    // Convert back to original coordinates
    sys.eigenvalues = eigenvalues_sqrt;  // omega^2 values
    sys.eigenvectors = M_inv_sqrt.inverse() * eigenvectors_sqrt;

    // Normalize eigenvectors
    for (int i = 0; i < 5; i++)
    {
        sys.eigenvectors.col(i).normalize();
    }
}

TimeEvolution evolveSystem(const OscillatorSystem& sys, double t_max, int num_steps)
{
    TimeEvolution evolution;

    // Transform initial conditions to normal mode coordinates
    MatrixXd M_inv_sqrt = MatrixXd::Zero(5, 5);
    for (int i = 0; i < 5; i++)
    {
        M_inv_sqrt(i, i) = 1.0 / sqrt(sys.masses(i));
    }

    VectorXd q0 = sys.eigenvectors.transpose() * M_inv_sqrt * sys.initial_positions;
    VectorXd q_dot0 = sys.eigenvectors.transpose() * M_inv_sqrt * sys.initial_velocities;

    // Time evolution
    evolution.positions = MatrixXd::Zero(num_steps, 5);

    for (int step = 0; step < num_steps; step++)
    {
        double t = (double)step / (num_steps - 1) * t_max;
        evolution.times.push_back(t);

        VectorXd q_t = VectorXd::Zero(5);
        for (int n = 0; n < 5; n++)
        {
            double omega = sqrt(sys.eigenvalues(n));
            if (omega < 1e-10)
            {
                q_t(n) = q0(n) + q_dot0(n) * t;
            }
            else
            {
                q_t(n) = q0(n) * cos(omega * t) + (q_dot0(n) / omega) * sin(omega * t);
            }
        }

        VectorXd x_t = M_inv_sqrt.inverse() * sys.eigenvectors * q_t;
        evolution.positions.row(step) = x_t.transpose();
    }

    return evolution;
}

VectorXd calculateCenterOfMass(const OscillatorSystem& sys, const MatrixXd& positions)
{
    VectorXd com = VectorXd::Zero(positions.rows());
    double total_mass = sys.masses.sum();

    for (int i = 0; i < positions.rows(); i++)
    {
        com(i) = ((sys.masses.transpose() * positions.row(i).transpose()) / total_mass)(0);
    }

    return com;
}
