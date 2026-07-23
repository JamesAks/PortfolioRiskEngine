add_library(QuantitativeLibrary INTERFACE)

target_link_libraries(QuantitativeLibrary

	INTERFACE

		Core
		Assets
		MarketDataServices
		PricingEngines
		RiskEngine
		
)