#include "../include/CovarianceMatrix.hpp"



// ----- CovarianceMatrix -----

// --- Private Members ---

bool CovarianceMatrix::validateMatrix(Eigen::MatrixXd matrix) const {

	// Check for valid shape.
	if (matrix.row(0).size() != matrix.col(0).size()) { return false; }
	
	// Check for negative diagonals. Diagonals of a covariance matrix are variance and variance can not be negative.
	for (int i = 0; i < matrix.row(0).size(); i++){

		if (matrix(i, i) < 0) { return false; }
	}

	// Check for symmetry
	if (matrix.transpose() != matrix) { return false; }

	return true;
}

// --- Public Members ---

CovarianceMatrix::CovarianceMatrix() : cov_matrix{ {} }, asset_labels{ {} }{}

CovarianceMatrix::CovarianceMatrix(Eigen::MatrixXd matrix): asset_labels { {}} {

	bool valid = validateMatrix(matrix);

	if (!valid) { throw "Atttempted to initialise invalid covariance matrix."; }

	cov_matrix = matrix;
}



CovarianceMatrix::CovarianceMatrix(std::vector<std::string> assets) : cov_matrix{ Eigen::MatrixXd::Zero(assets.size(), assets.size()) },
																		asset_labels{ assets } {}


CovarianceMatrix::CovarianceMatrix(size_t size) : asset_labels{}, cov_matrix{ Eigen::MatrixXd::Zero(size, size) } {}


double& CovarianceMatrix::operator()(size_t i, size_t j) { return cov_matrix(i, j); }


const Eigen::MatrixXd& CovarianceMatrix::matrixData() const { return cov_matrix;}


Eigen::MatrixXd CovarianceMatrix::inverse() const { return cov_matrix.inverse(); }

bool CovarianceMatrix::validate() { return validateMatrix(cov_matrix); }






