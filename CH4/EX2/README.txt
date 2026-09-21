EX2: SchrodPlot (1D quantum scattering through a stepped barrier)

Approach: The potential has five regions. It is 0 outside, V0 in the two outer barrier regions, and 2V0 in the middle region (boundaries at ±a and ±2a). I set hbar = m = 1, V0 = 1.5E, and let the user pick ka.
- In each region the wave function is a sum of e^{ikx} and e^{−ikx}.
- Continuity of psi and psi′ at the four boundaries gives 8 equations for 8 unknowns: r, A2, B2, A3, B3, A4, B4, t. I built that 8×8 complex matrix in Eigen (schrodinger.cpp) and solved it with householderQr().
- R = |r|^2 and T = |t|^2. The plot shows |psi(x)|^2 from −4a to 4a.

Findings:
- Since E < V0 everywhere inside, transmission is pure tunneling, so T is well below 1. Reflection sets up a standing-wave pattern to the left of the barrier, while the transmitted side is a flat |t|^2.
- T changes with ka. A larger ka means a larger E, so the barrier is relatively easier to cross (V/E is fixed, but the barrier is wider in wavelengths).
- The code doesn't print R + T. Checking that it equals 1 is the easiest way to confirm the matrix is right.
