add_library(QuantitativeLibrary INTERFACE)

target_link_libraries(QuantitativeLibrary

	INTERFACE
		RiskEngine
		MarketDataStore
		PricingEngine
		Core
)