project(dummy-plugin)
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)

get_target_property(PLUGIN_API_SOURCE_DIR plugin-api SOURCE_DIR)
include(${PLUGIN_API_SOURCE_DIR}/version-defs.cmake)

add_library(dummy-plugin MODULE
  dummyaddressvalidator.h
  dummyaddressvalidator.cpp
  dummyconnectioninfo.h
  dummyconnectioninfo.cpp
  dummyconnectionserializer.h
  dummyconnectionserializer.cpp
  dummyconnectionwidget.h
  dummyconnectionwidget.cpp
  dummymessagecommunicator.h
  dummymessagecommunicator.cpp
  dummyplugindefs.h
  dummyplugindefs.cpp
  dummyfacade.h
  dummyfacade.cpp
  description.json.in
)
target_link_libraries(dummy-plugin PUBLIC plugins-utils plugin-api Qt5::Core Qt5::Gui  Qt5::Widgets )
set_target_properties(dummy-plugin PROPERTIES LINKER_LANGUAGE CXX)

file(MAKE_DIRECTORY  "${CMAKE_CURRENT_BINARY_DIR}/Dummy.plugin")
configure_file(description.json.in "${CMAKE_CURRENT_BINARY_DIR}/description.json.pred")
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Dummy.plugin/description.json" INPUT "${CMAKE_CURRENT_BINARY_DIR}/description.json.pred" )

set_target_properties(dummy-plugin PROPERTIES
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON)

add_custom_command(TARGET dummy-plugin POST_BUILD
    COMMAND cmake -E copy $<TARGET_FILE:dummy-plugin> "${CMAKE_CURRENT_BINARY_DIR}/Dummy.plugin"
)
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Dummy.plugin" DESTINATION "plugins")

if(WIN32)
    target_compile_definitions(dummy-plugin PRIVATE WIN_EXPORT)
endif()
