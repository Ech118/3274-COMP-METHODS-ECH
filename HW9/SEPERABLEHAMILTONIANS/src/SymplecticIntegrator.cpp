#include "SymplecticIntegrator.h"

SymplecticIntegrator::SymplecticIntegrator(Genfun::GENFUNCTION Hin,
                                           const Classical::PhaseSpace & phaseSpace,
                                           double hin)
  : n(phaseSpace.dim()),
    h(hin),
    t(0.0),
    H(Hin.clone()),                 // own a copy so it can't dangle
    state(2 * phaseSpace.dim())     // (q, p) vector, size 2n
{
  const Classical::PhaseSpace::Component
    & qVar = phaseSpace.coordinates(),
    & pVar = phaseSpace.momenta();

  dHdq.reserve(n);
  dHdp.reserve(n);
  qIndex.reserve(n);
  pIndex.reserve(n);

  for (unsigned int i = 0; i < n; i++) {
    // Where each variable lives in the Argument vector
    qIndex.push_back(qVar[i].index());
    pIndex.push_back(pVar[i].index());

    // Gradients of H, built once and reused every step
    dHdq.push_back(Hin.partial(qVar[i]));
    dHdp.push_back(Hin.partial(pVar[i]));

    // Initial conditions
    state[qIndex[i]] = phaseSpace.startValue(qVar[i]);
    state[pIndex[i]] = phaseSpace.startValue(pVar[i]);
  }
}

void SymplecticIntegrator::step() {

  // Half kick: p_i -= (h/2) dH/dq_i
  for (unsigned int i = 0; i < n; i++)
    state[pIndex[i]] -= 0.5 * h * dHdq[i](state);

  // Full drift: q_i += h dH/dp_i   (uses the half-kicked p)
  for (unsigned int i = 0; i < n; i++)
    state[qIndex[i]] += h * dHdp[i](state);

  // Half kick again, with gradients re-evaluated at the new q
  for (unsigned int i = 0; i < n; i++)
    state[pIndex[i]] -= 0.5 * h * dHdq[i](state);

  t += h;
}

void SymplecticIntegrator::integrate(double tmax) {
  while (t < tmax) step();
}
