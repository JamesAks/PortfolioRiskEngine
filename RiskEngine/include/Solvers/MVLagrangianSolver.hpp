#ifndef MVLANGRANGIAN_SOLVER_HPP
#define MVLANGRANGIAN_SOLVER_HPP

#include "Solver.hpp"


#include <vector>



class CovarianceMatrix;


class MVLagrangianSolver : public Solver {


	// Mean-Variance Lagrangrian solver for a minimum variance portfolio given a target return.

		/* Calculates the weights a given portfolio needs to have a given return. Uses a lagrangian function expressed
			as matrices. The calculations take the form:

							[Σ    μ    1] [w]   [0]
							[μᵀ   0    0] [λ] = [t]
							[1ᵀ   0    0] [γ]   [1]

			where Σ is the covariance matrix of the portfolio, μ is the expected returns of the assets, w is a matrix of the weights,
			1 is a matrix of 1s, λ γ are both lagrangian multipliers (not needed to actually be known but needed for calculations),
			and t is the target return needed.

			This is in the form Ax = b therefore the calculation is solving for x and extracting the weights from the resulting matrix.*/
	
	private:

		double target_return = 0.05;

	public:

		std::vector<double> solve(const std::vector<double>&, const CovarianceMatrix&) override;
		
		bool changeTargetReturn(double);
};





#endif // !MVLANGRANGIAN_SOLVER_HPP