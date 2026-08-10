#ifndef SOLVERS_HPP
#define SOLVERS_HPP



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
		void reset();

		virtual ~Solver() = default;
};







#endif