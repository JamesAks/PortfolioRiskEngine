set(ASSETS_SOURCES

    src/Equities/Stock.cpp
	src/Equity.cpp
)

set(ASSETS_HEADERS

	include/Equities/Stock.hpp
	include/Equity.hpp
)

add_library(Assets ${ASSETS_SOURCES} ${ASSETS_HEADERS})

target_link_libraries(Assets

	PUBLIC
		Core
)


target_include_directories(Assets

	PUBLIC
		include
		src
)