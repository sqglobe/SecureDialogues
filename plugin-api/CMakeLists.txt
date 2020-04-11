project(plugin-api LANGUAGES CXX)
find_package(Qt5 COMPONENTS REQUIRED Widgets Core)

include(version-defs.cmake)

add_library(plugin-api STATIC
    export/pluginconnectioninfo.h
    export/pluginconnectionserializer.h
    export/pluginapierrorcodes.h
    export/pluginwidget.h
    export/recievedmessagesiterator.h
    export/pluginmessagecommunicator.h
    export/pluginfacade.h
    export/pluginapidefs.h
    export/pluginaddressvalidator.h
    export/export-definition.h
    inner/plugininnerdefs.h
    export/languages.h
    api-version.h.in
    plugin.schema.json
)

configure_file(api-version.h.in ${CMAKE_CURRENT_BINARY_DIR}/api-version.h @ONLY)
target_include_directories(plugin-api PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} )
set_target_properties(plugin-api PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(plugin-api PROPERTIES SCHEMA_FILE ${CMAKE_CURRENT_SOURCE_DIR}/plugin.schema.json)
