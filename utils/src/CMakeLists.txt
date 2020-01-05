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
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${BerkeleyDB_INCLUDE_DIRS})

add_library(utils-headers INTERFACE)
target_include_directories(utils-headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${BerkeleyDB_INCLUDE_DIRS})

