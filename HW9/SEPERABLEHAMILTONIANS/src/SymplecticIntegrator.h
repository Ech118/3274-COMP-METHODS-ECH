#pragma once

#include "QatGenericFunctions/AbsFunction.h"
#include "QatGenericFunctions/Argument.h"
#include "QatGenericFunctions/PhaseSpace.h"
#include <memory>
#include <vector>

class SymplecticIntegrator {
public:
    SymplecticIntegrator(Genfun::GENFUNCTION H,
                         const Classical::PhaseSpace & phaseSpace,
                         double h);

    // Advance the state by one leapfrog step of size h
    void step();

    // Step repeatedly until time reaches tmax
    void integrate(double tmax);

    // Accessors
    double time()   const { return t; }
    double q(unsigned int i) const { return state[qIndex[i]]; }
    double p(unsigned int i) const { return state[pIndex[i]]; }
    const Genfun::Argument & getState() const { return state; }
    double energy() const { return (*H)(state); }

private:
    unsigned int n;        // degrees of freedom
    double       h;        // fixed time step
    double       t;        // current time

    std::unique_ptr<const Genfun::AbsFunction>    H;        // owned copy: H.clone()
    std::vector<Genfun::Derivative>            dHdq;     // ∂H/∂q_i, i = 0..n-1
    std::vector<Genfun::Derivative>            dHdp;     // ∂H/∂p_i, i = 0..n-1

    std::vector<unsigned int>                     qIndex;   // slot of q_i in the state vector
    std::vector<unsigned int>                     pIndex;   // slot of p_i in the state vector

    Genfun::Argument                              state;    // current (q, p), size 2n
};