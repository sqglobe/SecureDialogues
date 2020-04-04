project(plugins-support LANGUAGES CXX)

find_package(Boost  REQUIRED filesystem)


SET(PLUGIN_SUPPORT_HEADER
      not_owned.h
      owneddeletior.h
      plugininterface.h
      pluginwidgetwrapper.h
      pluginmessagecommunicatorwrapper.h
      pluginscontainer.h
      plugindiscovery.h
      support-functions.h
)

SET(PLUGIN_SUPPORT_SOURCES
      plugininterface.cpp
      pluginwidgetwrapper.cpp
      pluginmessagecommunicatorwrapper.cpp
      pluginscontainer.cpp
      plugindiscovery.cpp
)
add_library(plugins-support-headers INTERFACE)
target_include_directories(plugins-support-headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(plugins-support-headers INTERFACE plugin-api)

add_library(plugins-support  STATIC ${PLUGIN_SUPPORT_SOURCES} ${PLUGIN_SUPPORT_HEADER})
target_link_libraries(plugins-support PUBLIC plugins-support-headers)
target_link_libraries(plugins-support PRIVATE  Boost::filesystem ${CMAKE_DL_LIBS} boost-dll-headers)

set_property(TARGET plugins-support PROPERTY POSITION_INDEPENDENT_CODE ON)
