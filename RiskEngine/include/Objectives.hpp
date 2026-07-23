#ifndef OBJECTIVES_HPP
#define OBJECTIVES_HPP

#include <Eigen/dense>
#include <vector>



class CovarianceMatrix;

class Objective {

	public:
		
		virtual double evaluate(const std::vector<double>&, const std::vector<double>&, const CovarianceMatrix&) const = 0;
		virtual ~Objective() = default;
};

struct MaximiseReturn: public Objective{

	public:
		
		double evaluate(const std::vector<double>&, const std::vector<double>&, const CovarianceMatrix&) const override;
};


struct MaximiseSharpeRatio: public Objective {

	public:

		double evaluate(const std::vector<double>&, const std::vector<double>&, const CovarianceMatrix&) const override;
};


struct MinimiseVolatility: public Objective {

	public:

		double evaluate(const std::vector<double>&, const std::vector<double>&, const CovarianceMatrix&) const override;
};



#endif 