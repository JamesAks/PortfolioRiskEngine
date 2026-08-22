#include "EfficientFrontier.hpp"



EfficientFrontier::EfficientFrontier(std::array<EfficientFrontierPoint, 50> points) : efficient_frontier{points} {}


const std::array<EfficientFrontierPoint, 50>& EfficientFrontier::data() const { return efficient_frontier; }


double EfficientFrontier::minimumReturn() const{

	return efficient_frontier[0].expected_return;
}


double EfficientFrontier::maximumReturn() const{

	return efficient_frontier[efficient_frontier.size() - 1].expected_return;
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


size_t EfficientFrontier::size() const { return efficient_frontier.size(); }