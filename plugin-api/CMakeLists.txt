project(plugin-api LANGUAGES CXX)
find_package(Qt5 COMPONENTS REQUIRED Widgets)
SET(PLUGIN_API_SOURCES
)

SET(PLUGIN_API_HEADERS
    export/pluginconnectioninfo.h
    export/pluginconnectionserializer.h
    export/pluginapierrorcodes.h
    export/pluginwidget.h
    export/recievedmessagesiterator.h
    export/pluginmessagecommunicator.h
    export/pluginfacade.h
    export/pluginapidefs.h
    export/pluginaddressvalidator.h
    inner/plugininnerdefs.h
    export/languages.h
)

add_library(plugin-api STATIC ${PLUGIN_API_SOURCES} ${PLUGIN_API_HEADERS})

target_include_directories(plugin-api PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Qt5Widgets_INCLUDE_DIRS} )
set_target_properties(plugin-api PROPERTIES LINKER_LANGUAGE CXX)
