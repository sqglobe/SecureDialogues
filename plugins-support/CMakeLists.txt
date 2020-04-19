project(plugins-support LANGUAGES CXX)

find_package(Boost  REQUIRED filesystem)
find_package(nlohmann_json REQUIRED)
find_package(nlohmann_json_schema_validator REQUIRED)
find_package(Qt5 COMPONENTS REQUIRED Widgets)

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
target_link_libraries(plugins-support PRIVATE  Boost::filesystem ${CMAKE_DL_LIBS} nlohmann_json_schema_validator spdlog-headers  dynalo-headers CompilationFlags Qt5::Widgets)


set_property(TARGET plugins-support PROPERTY POSITION_INDEPENDENT_CODE ON)

get_target_property(PLUGIN_API_SOURCE_DIR plugin-api SOURCE_DIR)

install(FILES  ${PLUGIN_API_SOURCE_DIR}/plugin.schema.json DESTINATION ".")
