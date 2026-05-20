add_library(Core src/Core.cpp include/Core.hpp)

target_include_directories(Core 

	PUBLIC
		$(SOURCE_DIR)/include)