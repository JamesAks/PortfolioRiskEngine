add_library(QuantitativeLibrary INTERFACE)

target_link_libraries(QuantitativeLibrary

	INTERFACE
		RiskEngine
		MarketDataServices
		PricingEngine
		Core
)