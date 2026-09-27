Chapter 5 Exercise 2:

Notes:
Integrate the nonlinear pendulum ODE to find the period correction factor f(θ_max) for large-amplitude oscillations.

- Small amplitude period: T_0 = 2π√(l/g)
- Large amplitude: T = T_0 * f(θ_max)
- f(θ_max) > 1 because nonlinear effects increase period

ODE:
d²θ/dt² = -(g/l)sin(θ)
Initial conditions: θ(0) = θ_max, dθ/dt(0) = 0

Solving:
1. Rewrite as 2nd-order system: dθ/dt = v, dv/dt = -(g/l)sin(θ)
2. Use RKIntegrator to solve from t=0 until θ crosses 0
3. Quarter-period T/4 = time to reach θ=0
4. Full period T = 4 * (T/4)
5. Correction factor f = T / T_0

Solution:
- At θ_max = 15° (π/12): f ≈ 1.01 (small correction)
- At θ_max = 90° (π/2): f ≈ 1.07 (moderate correction)
- At θ_max = 180° (11π/12): f ≈ 1.20 (large correction)

Nonlinear effects become significant for large amplitudes. The curve shows how dramatically the period increases as amplitude approaches 180°. 
