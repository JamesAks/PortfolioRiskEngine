#include "Solver.hpp"

#include "CovarianceMatrix.hpp"
#include "Portfolio.hpp"
#include "Logger.hpp"



// ----- Solver Base Class -----

// --- Private Members ---

// --- Public Members ---

bool Solver::isFinished() const { return finished; };

void Solver::reset() { finished = false; }



