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
		double score = 0;

	public:

		std::vector<double> optimise(const std::vector<double>& a_returns, const CovarianceMatrix& c_matrix) {

			score = 0;
			std::vector<double> solution;

			while (!solver.isFinished()) {

				auto current_solution = solver.solve(a_returns, c_matrix);
				auto current_solution_score = objective.evaluate(current_solution, a_returns, c_matrix);

				if (current_solution_score > score) { 
					
					score = current_solution_score;
					solution = current_solution;
				}
			}

			// Reset the solver for later use.
			solver.reset();

			return solution;
		}

		Solver& viewSolver() { return solver; }

		ObjectivePolicy& viewObjective() { return objective; }

		double currentScore() { return score; }
};

#endif // !OPTIMIZER_HPP
