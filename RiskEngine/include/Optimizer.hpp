#ifndef OPTIMIZER_HPP
#define OPTIMIZER_HPP

#include "CovarianceMatrix.hpp"

#include <vector>


struct PortfolioData {

	std::vector<double> weights;
	double expected_return;
	double volatility;
};



template<typename ObjectivePolicy, typename Solver>
class Optimizer {

	private:

		ObjectivePolicy objective;
		Solver solver;

	public:

		std::vector<double> optimise(const std::vector<double>& a_returns, const CovarianceMatrix& c_matrix) {

			int attempts = 0;

			double best_solution_score = 0;
			std::vector<double> solution;

			while (!solver.isFinished()) {

				auto current_solution = solver.solve(a_returns, c_matrix);
				auto current_solution_score= objective.evaluate(current_solution, a_returns, c_matrix);

				if (current_solution_score > best_solution_score) { 
					
					best_solution_score = current_solution_score;
					solution = current_solution;
				}
			}

			// Reset the solver for later use.
			solver.reset();

			return solution;
		}

		Solver& viewSolver() { return solver; }

		ObjectivePolicy& viewObjective() { return objective; }
};

#endif // !OPTIMIZER_HPP
