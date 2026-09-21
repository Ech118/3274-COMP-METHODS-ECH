EX3: LegendreRoots

Approach: The user picks the order l (1–10). I build LegendrePolynomial(l) and find every zero with Genfun's NewtonRaphson.
- Newton's method needs a good starting point, so the m-th guess uses the standard asymptotic formula: (1 − 1/(8l^2) + 1/(8l^3))·cos((4m−1)pi/(4l+2)).
- The zeros are printed to 10 digits. The plot shows the polynomial with the zeros as red dots on the x-axis.

Findings:
- P_l has exactly l distinct real zeros, all inside (−1, 1), symmetric about 0. Odd l includes a zero at x = 0.
- The asymptotic guesses landed close enough that Newton converged to a different zero for each m, so no roots were missed or duplicated.
- The zeros interlace with those of P_l-1. For l = 2 they are +-0.5774 (+-1/sqrt(3)).