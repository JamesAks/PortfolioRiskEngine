#include <vector>
#include <array>



struct EfficientFrontierPoint {

	std::vector<double> weights;
	double expected_return;
	double volatitity;
};


class EfficientFrontier {

	private:

		std::array<EfficientFrontierPoint,50> efficient_frontier;

	public:

		EfficientFrontier(std::array<EfficientFrontierPoint, 50>);

		const std::array<EfficientFrontierPoint, 50>& data() const;

		double minimumReturn() const;
		double maximumReturn() const;

		double minimumVolatility() const;
		double maximumVolatility() const;

		size_t size() const;

};