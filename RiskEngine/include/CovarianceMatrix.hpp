#ifndef COVARIANCE_MATRIX_CPP
#define COVARIANCE_MATRIX_CPP

#include <Eigen/Dense>

// Wrapper that contains the covariance matrix data need for computing portfolio variance.
class CovarianceMatrix {

	private: 

		Eigen::MatrixXd cov_matrix;
		std::vector<std::string> asset_labels;

		bool validateMatrix(Eigen::MatrixXd) const;

	public:

		CovarianceMatrix();
		CovarianceMatrix(Eigen::MatrixXd);
		CovarianceMatrix(std::vector<std::string>);
		CovarianceMatrix(size_t);

		double& operator()(size_t, size_t);


		const Eigen::MatrixXd& matrixData() const;
		Eigen::MatrixXd inverse() const;

		bool validate();

		
	
};

#endif // !COVARIANCE_MATRIX_CPP