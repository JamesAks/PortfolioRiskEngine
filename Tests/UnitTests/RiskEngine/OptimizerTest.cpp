#include <catch2/catch_test_macros.hpp>
#include "Optimizer.hpp"
#include "Solvers/MVLagrangianSolver.hpp"
#include "Solvers/GMVPLagrangianSolver.hpp"
#include "Objectives.hpp"



TEST_CASE("Optimizer can produce optimised weights for a given target return.", "[RiskEngine]") {

	// Arrange
	std::vector<double> test_returns = { 0.0447, 0.1593, 0.1759, 0.1741, 0.1660 };
	std::vector<double> test_answer = { 0.907396, 0.225525, 0.157598, -0.738521, 0.448002 };

	Eigen::MatrixXd data(5, 5);

	data << 0.06527, -0.02023, -0.00939, 0.04021, -0.00334,
		-0.02023, 0.02307, -0.01094, -0.01518, 0.01200,
		-0.00939, -0.01094, 0.03054, -0.01919, -0.02073,
		0.04021, -0.01518, -0.01919, 0.04731, 0.00889,
		-0.00334, 0.01200, -0.02073, 0.00889, 0.02389;

	CovarianceMatrix test_cov_matrix{ data };

	Optimizer<MinimiseVolatility, MVLagrangianSolver> sut;

	// Act
	auto result = sut.optimise(test_returns, test_cov_matrix);

	// Assert
	double returns = 0;
	for (int i = 0; i < result.size(); i++) {

		returns += result[i] * test_returns[i];
	}

	double sum = 0;
	for (auto weight : result) { sum += weight; }

	REQUIRE((roundf(returns * 100) / 100) == float(0.05));
	REQUIRE((roundf(sum * 100) / 100) == float(1));
}

TEST_CASE("Optimizer can produce the global minimum variance portfolio.", "[RiskEngine]") {

	// Arrange
	std::vector<double> test_returns = { 0.0447, 0.1593, 0.1759, 0.1741, 0.1660 };
	std::vector<double> test_answer = { 0.907396, 0.225525, 0.157598, -0.738521, 0.448002 };

	Eigen::MatrixXd data(5, 5);

	data << 0.06527, -0.02023, -0.00939, 0.04021, -0.00334,
		-0.02023, 0.02307, -0.01094, -0.01518, 0.01200,
		-0.00939, -0.01094, 0.03054, -0.01919, -0.02073,
		0.04021, -0.01518, -0.01919, 0.04731, 0.00889,
		-0.00334, 0.01200, -0.02073, 0.00889, 0.02389;

	CovarianceMatrix test_cov_matrix{ data };

	Optimizer<MinimiseVolatility, GMVPLagrangianSolver> sut;

	// Act
	auto result = sut.optimise(test_returns, test_cov_matrix);

	// Assert
	double returns = 0;
	for (int i = 0; i < result.size(); i++) {

		returns += result[i] * test_returns[i];
	}

	double sum = 0;
	for (auto weight : result) { sum += weight; }

	REQUIRE((roundf(returns * 100) / 100) == float(0.05));
	REQUIRE((roundf(sum * 100) / 100) == float(1));
}
	