Chapter 5 Exercise 1

Notes:
- Mesh spacing h = 1/n
- X-axis: log(h) = log(1/n) (negative for n > 1)
- Y-axis: log(|error|), where error = |approximate - analytical|
- Log-log plot reveals convergence order as slope

Quad. Rules Tested:
1. Rectangle Rule (Midpoint)
   - Slope ≈ 1 on log-log plot
   
2. Trapezoid Rule
   - Slope ≈ 2 on log-log plot
   - ~4× better than rectangle at each doubling of n
   
3. Simpson's Rule
   - Slope ≈ 4 on log-log plot
   - ~16× better than rectangle at each doubling of n

INTEGRALS COMPUTED:
- ∫0->1 tanh(x) dx = ln(cosh(1)) ≈ 0.4338
- ∫0.01->0.99 √(coth(x)) dx (singularities at 0, 1)

On log-log scale, error drops as a straight line with slope = convergence order. I had never used log-log scale but it is easy to see convergence for computational quadrature. So a steeper slope = faster convergence = fewer intervals needed for same accuracy.

Figuring out setting up horizontal graphs was a little bit of a challenge, but worth it.