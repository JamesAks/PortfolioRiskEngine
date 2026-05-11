#include "../include/CovarianceMatrix.hpp"



CovarianceMatrix::CovarianceMatrix(std::vector<std::string> assets) {

	size_t size = assets.size();
	cov_matrix = Eigen::MatrixXd::Zero(size, size);
	asset_labels = assets;
}


double& CovarianceMatrix::operator()(size_t i, size_t j) { return cov_matrix(i, j); };

Eigen::MatrixXd CovarianceMatrix::matrixData() const {

	return cov_matrix;
}

void CovarianceMatrix::validateMatrix() const {

	return;
}


