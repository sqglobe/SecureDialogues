project(utils)

set(SOURCES 
    main.cpp
)


set(HEADERS
    utils/interfaces/objectserializer.h
    utils/interfaces/objectdeserializer.h
)


add_library(utils STATIC ${SOURCES} ${HEADERS})
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

