project(plugins-support LANGUAGES CXX)

find_package(Boost  REQUIRED filesystem)


SET(PLUGIN_SUPPORT_HEADER
      not_owned.h
      owneddeletior.h
      plugininterface.h
)

SET(PLUGIN_SUPPORT_SOURCES
      plugininterface.cpp
)

add_library(plugins-support  STATIC ${PLUGIN_SUPPORT_SOURCES} ${PLUGIN_SUPPORT_HEADER})

target_include_directories(plugins-support  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
set_target_properties(plugins-support  PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(plugins-support PUBLIC plugin-api Boost::filesystem)
set_property(TARGET plugins-support PROPERTY POSITION_INDEPENDENT_CODE ON)
