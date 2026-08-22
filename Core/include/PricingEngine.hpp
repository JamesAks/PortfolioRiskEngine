#ifndef PRICING_ENGINE_HPP
#define PRICING_ENGINE_HPP



class PricingEngine {

	private:

	public:

		class results;
		double calculatePrice();

		virtual ~PricingEngine() = default;
};



#endif