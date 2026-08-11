#include "Solver.hpp"

class CovarianceMatrix;

class GMVPLagrangianSolver : public Solver {

	// Global Minimum Variance Portfolio Lagrangian solver for the highest return portfolio with the least amount of volitility.

	private:


	public:

		std::vector<double> solve(const std::vector<double>&, const CovarianceMatrix&) override;

};