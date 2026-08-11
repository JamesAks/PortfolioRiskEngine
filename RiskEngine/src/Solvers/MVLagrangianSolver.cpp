#include "Solvers/MVLagrangianSolver.hpp"
#include "CovarianceMatrix.hpp"
#include "Logger.hpp"



// -----  Mean-Variance Lagrangrian solver -----

// --- Private Members ---

// --- Public Members ---

std::vector<double> MVLagrangianSolver::solve(const std::vector<double>& expected_returns, const CovarianceMatrix& cov_matrix){

	/* Calculates the weights a given portfolio needs to have a given return. Uses a lagrangian function expressed
			as matrices. The calculations take the form:
							
							[Σ    μ    1] [w]   [0]
							[μᵀ   0    0] [λ] = [t]
							[1ᵀ   0    0] [γ]   [1]

			where Σ is the covariance matrix of the portfolio, μ is the expected returns of the assets, w is a matrix of the weights,
			1 is a matrix of 1s, λ γ are both lagrangian multipliers (not needed to actually be known but needed for calculations),
			and t is the target return needed.

			This is in the form Ax = b therefore the calculation is solving for x and extracting the weights from the resulting matrix.*/


	size_t numb_of_assets = expected_returns.size();
	size_t new_size = numb_of_assets + 2;
	
	// Create target matrix.

	Eigen::MatrixXd target_matrix = Eigen::MatrixXd::Zero(new_size,1);
	target_matrix.block(numb_of_assets, 0 , 2, 1) << target_return, 1;

	// Create equation matrix.
	Eigen::MatrixXd returns_matrix{1,numb_of_assets};
	for (int i = 0; i < numb_of_assets; i++) {  returns_matrix(0, i) = expected_returns[i]; }

	Eigen::MatrixXd equation_matrix = Eigen::MatrixXd::Ones(new_size, new_size); 

	equation_matrix.block(0, 0, numb_of_assets, numb_of_assets) = cov_matrix.matrixData();
	equation_matrix.block(numb_of_assets,0, 1, numb_of_assets) = returns_matrix;
	equation_matrix.block(0, numb_of_assets, numb_of_assets, 1) = returns_matrix.transpose();
	equation_matrix.block(numb_of_assets, numb_of_assets, 2, 2).setZero();

	// Uses decomposition to solve for x instead of multipying by the inverse matrix.

	Eigen::MatrixXd calculation = equation_matrix.colPivHouseholderQr().solve(target_matrix);

	// Extracting the relevant data.
	Eigen::MatrixXd solution = calculation.block(0, 0, numb_of_assets, 1);

	auto weights = std::vector<double>(solution.data(), solution.data() + numb_of_assets);

	finished = true;

	return weights;
}


bool MVLagrangianSolver::changeTargetReturn(double new_target) {

	target_return = new_target;
	return true;
}




