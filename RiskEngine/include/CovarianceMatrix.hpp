#ifndef COVARIANCE_MATRIX_CPP
#define COVARIANCE_MATRIX_CPP



#include <Eigen/Dense>

// Wrapper that contains the covariance matrix data need for computing portfolio variance.
class CovarianceMatrix {

	private: 

		Eigen::MatrixXd cov_matrix;
		std::vector<std::string> asset_labels;

	public:

		CovarianceMatrix();
		CovarianceMatrix(std::vector<std::string>);
		double& operator()(size_t, size_t);

		const Eigen::MatrixXd& matrixData() const;
		Eigen::MatrixXd inverse() const;
		void validateMatrix() const;
		
	
};

#endif // !COVARIANCE_MATRIX_CPP