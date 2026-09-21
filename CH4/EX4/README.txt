EX4: System (100 coupled beads on a string)

Approach: N = 100 beads of mass M = 1 are joined by springs of constant K = 1, with the two end beads tied to fixed walls.
- Newton's equations become m·x'' = −S·x, where S is a 100×100 tridiagonal matrix with 2K on the diagonal and −K on the off-diagonals.
- The normal modes are the eigenvectors of S, and omega^2 = eigenvalue/M. I used Eigen's SelfAdjointEigenSolver, printed the lowest five omegas, and plotted the five mode shapes with PlotProfile.
- I flipped signs so bead 1 always moves positive, which keeps the mode shapes consistent.

Findings:
- The modes are sine waves, so the pth mode has p−1 nodes.
- The frequencies match the analytic result omega_p = 2 sin(p(pi) / 202). That is ~= 0.0311, 0.0622, 0.0933, 0.1244, 0.1555 for p = 1–5.
- For low modes the frequencies are nearly evenly spaced, omega ~= p(pi)/101. That is the continuous-string limit.