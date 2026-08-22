#include "Solvers/GMVPLagrangianSolver.hpp"
#include "CovarianceMatrix.hpp"

std::vector<double> GMVPLagrangianSolver::solve(const std::vector<double>& expected_returns, const CovarianceMatrix& cov_matrix){

	/* Calculates the weights of the portfolio with the highest return at the least volatility. This solver assumes that the portfolio
		is unconstrained in terms of weights, i.e.shorting is allowed. Uses a lagrangian function expressed as matrices. The calculations
		take the form:

							[2Σ  1] [w]  =  [0]
							[1ᵀ  0] [λ]     [1]

		where Σ is the covariance matrix of the portfolio, w is a matrix of the weights, 1 is a matrix of 1s and λ is the lagrangian
		multiplier (not needed to actually be known but needed for calculations).

		This is in the form Ax = b therefore the calculation is solving for x and extracting the weights from the resulting matrix .*/
				
	size_t numb_of_assets = expected_returns.size();
	size_t new_size = numb_of_assets + 1;
	
	// Construct target matrix.
	Eigen::MatrixXd target_matrix = Eigen::MatrixXd::Zero(new_size, 1);
	target_matrix(numb_of_assets,0) =  1;

	// Create equation matrix.
	Eigen::MatrixXd equation_matrix = Eigen::MatrixXd::Ones(new_size, new_size);

	equation_matrix.block(0, 0, numb_of_assets, numb_of_assets) = 2 * cov_matrix.matrixData();
	equation_matrix(numb_of_assets, numb_of_assets) = 0;

	// Uses decomposition to solve for x instead of multipying by the inverse matrix.

	Eigen::MatrixXd calculation = equation_matrix.colPivHouseholderQr().solve(target_matrix);

	// Extracting the relevant data.
	Eigen::MatrixXd solution = calculation.block(0, 0, numb_of_assets, 1);

	auto weights = std::vector<double>(solution.data(), solution.data() + numb_of_assets);

	finished = true;

	return weights;
}