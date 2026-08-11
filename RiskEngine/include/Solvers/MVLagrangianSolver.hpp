#ifndef MVLANGRANGIAN_SOLVER_HPP
#define MVLANGRANGIAN_SOLVER_HPP

#include "Solver.hpp"

#include <vector>



class CovarianceMatrix;


class MVLagrangianSolver : public Solver {


	// Mean-Variance Lagrangrian solver for a minimum variance portfolio given a target return.

	private:

		double target_return = 0.05;

	public:

		std::vector<double> solve(const std::vector<double>&, const CovarianceMatrix&) override;
		
		bool changeTargetReturn(double);
};





#endif // !MVLANGRANGIAN_SOLVER_HPP