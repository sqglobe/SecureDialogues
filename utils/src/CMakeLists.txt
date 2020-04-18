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
    utils/base64.h
    utils/base64.cpp
)


add_library(utils STATIC ${SOURCES} ${HEADERS})
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${BerkeleyDB_INCLUDE_DIRS})
target_link_libraries(utils PRIVATE CompilationFlags)
set_property(TARGET utils PROPERTY POSITION_INDEPENDENT_CODE ON)
add_library(utils-headers INTERFACE)
target_include_directories(utils-headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${BerkeleyDB_INCLUDE_DIRS})

