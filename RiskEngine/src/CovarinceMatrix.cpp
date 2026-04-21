#include "../include/CovarianceMatrix.hpp"



CovarianceMatrix::CovarianceMatrix() :cov_matrix{ Eigen::MatrixXd::Zero(1,1) }, asset_labels{ {""} }{}


CovarianceMatrix::CovarianceMatrix(std::vector<std::string> assets) {

	size_t size = assets.size();
	cov_matrix = Eigen::MatrixXd::Zero(size, size);
	asset_labels = assets;
}


Eigen::MatrixXd CovarianceMatrix::data() {

	return cov_matrix;
}


double& CovarianceMatrix::operator()(size_t i, size_t j) { return cov_matrix(i, j); };


void CovarianceMatrix::validate() {

}