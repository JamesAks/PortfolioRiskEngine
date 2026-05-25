

set(CORE_SOURCES
    
    src/Asset.cpp
    src/Core.cpp
    src/Logger.cpp
    src/Portfolio.cpp
    src/Position.cpp
    src/Statistics.cpp
)


set(CORE_HEADERS

    include/Asset.hpp
    include/Core.hpp
    include/Logger.hpp
    include/Portfolio.hpp
    include/Position.hpp
    include/Statistics.hpp
    include/HistoricData.hpp
    include/TimeSeries.hpp
)

add_library(Core STATIC ${CORE_SOURCES} ${CORE_HEADERS})

target_include_directories(Core 

	PUBLIC
		include
)
