EX1: BasicPlot 

Approach: One program plots any of seven functions, chosen by a command-line argument. Each function is a plain C++ double f(double), wrapped in a Genfun::F1D and drawn with PlotFunction1D. A switch sets the title and axis rectangle for each case.
- The functions are sin(x), sin(5x), sin(x²), e⁻ˣ sin(x), P₃²(x) = 15x(1−x²) on [−1,1], x^x, and d/dx x^x.
- The derivative is not coded by hand. It comes from F1D::prime(), which is the part that used the Genfun library most.

Findings:
- sin(5x) oscillates five times faster than sin(x).
- sin(x²) is a chirp: the oscillation speeds up as x grows.
- e⁻ˣ sin(x) is a sine wave under a decaying exponential envelope.
- P₃²(x) has zeros at x = 0 and ±1 and is odd.
- x^x has a minimum at x = 1/e ≈ 0.37, where its value is about 0.69. The derivative x^x(ln x + 1) crosses zero exactly there, so the numeric prime() plot should agree with the analytic result.
