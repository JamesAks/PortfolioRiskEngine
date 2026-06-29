set(ASSETS_SOURCES

    src/Stock.cpp
)

set(ASSETS_HEADERS

	include/Stock.hpp
)

add_library(Assets ${ASSETS_SOURCES} ${ASSETS_HEADERS})

target_link_libraries(Assets

	PUBLIC
		Core
)


target_include_directories(Assets

	PUBLIC
		include
)