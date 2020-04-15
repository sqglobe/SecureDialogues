project(gmail-plugin)
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)
find_package(Qt5LinguistTools REQUIRED)
find_package(nlohmann_json REQUIRED)

include(${TOP_DIR}/secrets.txt)
get_target_property(PLUGIN_API_SOURCE_DIR plugin-api SOURCE_DIR)
include(${PLUGIN_API_SOURCE_DIR}/version-defs.cmake)

add_library(gmail-plugin MODULE
    gmailplugindefs.cpp
    gmailconnectioninfo.cpp
    gmailconnectionserializer.cpp
    gmailconnectionwidget.cpp
    gmailmessagecommunicator.cpp
    gmailfacade.cpp
    gmail-communication/gmailapi.cpp
    gmail-communication/gmailoauth.cpp
    gmailaddressvalidator.h

    gmailconnectioninfo.h
    gmailplugindefs.h
    gmailconnectionserializer.h
    gmailconnectionwidget.h
    gmailmessagecommunicator.h
    gmailfacade.h
    gmail-communication/gmailapi.h
    gmail-communication/gmailoauth.h
    gmail-communication/gmailexceptions.h
    gmailaddressvalidator.cpp

    description.json.in
    )

file(MAKE_DIRECTORY  "${CMAKE_CURRENT_BINARY_DIR}/Gmail.plugin")
configure_file(description.json.in "${CMAKE_CURRENT_BINARY_DIR}/description.json.pred")
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Gmail.plugin/description.json" INPUT "${CMAKE_CURRENT_BINARY_DIR}/description.json.pred" )
add_internationals(gmail-plugin  ru_RU.UTF-8 "locale" "${CMAKE_CURRENT_BINARY_DIR}/Gmail.plugin/locale")

qt5_wrap_cpp(WRAP_MOC_GUI gmailconnectionwidget.h)

qt5_wrap_ui(UI_WRAP gmailconnectionwidget.ui)
qt5_create_translation(QM_FILES gmailconnectionwidget.ui ${GMAIL_PLUGIN_HEADERS} ${GMAIL_PLUGIN_SOURCES} gmail_plugin_ru_RU.ts)

target_sources(gmail-plugin PRIVATE  ${WRAP_MOC_GUI} ${UI_WRAP} ${QM_FILES})
set_target_properties(gmail-plugin PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(gmail-plugin  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/submodules/spdlog/include)
target_link_libraries(gmail-plugin PUBLIC plugin-api curl-wrapper nlohmann_json::nlohmann_json plugins-utils utils spdlog-headers  Qt5::Core Qt5::Gui  Qt5::Widgets )

if(WIN32)
    target_link_libraries(gmail-plugin PUBLIC intl)
endif()
set_target_properties(gmail-plugin PROPERTIES
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON)

add_custom_command(TARGET gmail-plugin POST_BUILD
    COMMAND cmake -E copy $<TARGET_FILE:gmail-plugin> "${CMAKE_CURRENT_BINARY_DIR}/Gmail.plugin"
    COMMAND ${CMAKE_COMMAND} -E copy "${QM_FILES}" "${CMAKE_CURRENT_BINARY_DIR}/Gmail.plugin/locale"
)
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Gmail.plugin" DESTINATION "plugins")

if(WIN32)
    target_compile_definitions(gmail-plugin PRIVATE WIN_EXPORT)
endif()
