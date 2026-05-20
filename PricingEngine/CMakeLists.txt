add_library(PricingEngine src/PricingEngine.cpp include/PricingEngine.hpp)

target_include_directories(PricingEngine

	PUBLIC 
		$(SOURCE_DIR)/include)