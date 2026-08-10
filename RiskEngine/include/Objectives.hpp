#ifndef OBJECTIVES_HPP
#define OBJECTIVES_HPP

#include <Eigen/dense>
#include <vector>



class CovarianceMatrix;

class Objective {

	public:
		
		virtual double evaluate(const std::vector<double>&, const std::vector<double>&, const CovarianceMatrix&) = 0;
		virtual ~Objective() = default;
};

struct MaximiseReturn: public Objective{

	public:
		
		double evaluate(const std::vector<double>&, const std::vector<double>&, const CovarianceMatrix&) override;
};


struct MaximiseSharpeRatio: public Objective {

	private:

		double risk_free_rate = 0;

	public:

		double evaluate(const std::vector<double>&, const std::vector<double>&, const CovarianceMatrix&) override;
		void changeRiskFreeRate(double);
};


struct MinimiseVolatility: public Objective {

	private:

		double volatility = 0;

	public:

		double evaluate(const std::vector<double>&, const std::vector<double>&, const CovarianceMatrix&) override;
		double viewScore() const;
};

#endif 