
add_library(PricingEngines 

src/PricingEngine.cpp 

include/PricingEngine.hpp)

target_include_directories(PricingEngines

	PUBLIC 
		$(SOURCE_DIR)/include)