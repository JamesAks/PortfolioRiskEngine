#ifndef SOLVERS_HPP
#define SOLVERS_HPP

#include "CovarianceMatrix.hpp"

#include <Eigen/dense>
#include <vector>



class CovarianceMatrix;

class Solver {

	// Base class for solvers.

	protected:

		bool finished = false;

	public:

		virtual std::vector<double> solve(const std::vector<double>&, const CovarianceMatrix&) = 0;

		bool isFinished() const;

		virtual ~Solver() = default;
};







#endif