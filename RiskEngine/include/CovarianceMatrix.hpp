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

		Eigen::MatrixXd data();
		void validate();

};