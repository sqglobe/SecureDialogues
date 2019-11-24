project(utils)

find_package(BerkeleyDB REQUIRED)

set(SOURCES 
    utils/dbfactory.cpp
)


set(HEADERS
    utils/interfaces/objectserializer.h
    utils/interfaces/objectdeserializer.h
    utils/eventqueue.h
    utils/dbfactory.h
)


add_library(utils STATIC ${SOURCES} ${HEADERS})
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries(utils Oracle::BerkeleyDB)

