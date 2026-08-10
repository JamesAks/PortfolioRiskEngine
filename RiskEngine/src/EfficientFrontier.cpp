#include "EfficientFrontier.hpp"



EfficientFrontier::EfficientFrontier(std::array<EfficientFrontierPoint, 50> points) : efficient_frontier{points} {}


const std::array<EfficientFrontierPoint, 50>& EfficientFrontier::data() const { return efficient_frontier; }


double EfficientFrontier::minimumReturn() const{

	double minimum = efficient_frontier[0].expected_return;
	for (auto& point : efficient_frontier) {

		if (point.expected_return < minimum) { minimum = point.expected_return; }
	}

	return minimum;
}


double EfficientFrontier::maximumReturn() const{

	double maximum = 0;
	for (auto& point : efficient_frontier) {

		if (point.expected_return > maximum) { maximum = point.expected_return; }
	}

	return maximum;
}


double EfficientFrontier::minimumVolatility() const{
	
	double minimum = efficient_frontier[0].volatitity;
	for (auto& point : efficient_frontier) {

		if (point.volatitity < minimum) { minimum = point.volatitity; }
	}

	return minimum;
}


double EfficientFrontier::maximumVolatility() const{
	
	double maximum = 0;
	for (auto& point : efficient_frontier) {

		if (point.volatitity > maximum) { maximum = point.volatitity; }
	}

	return maximum;
}