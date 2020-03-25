project(gmail-plugin)
find_package(persistent-storage REQUIRED)
find_package(BerkeleyDB REQUIRED)
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)
find_package(Qt5LinguistTools REQUIRED)

include(${TOP_DIR}/secrets.txt)

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
    )

add_internationals(gmail-plugin  ru_RU.UTF-8 "locale" "${LOCALE_FILES_DIR}")

qt5_wrap_cpp(WRAP_MOC_GUI gmailconnectionwidget.h)

qt5_wrap_ui(UI_WRAP gmailconnectionwidget.ui)
qt5_create_translation(QM_FILES gmailconnectionwidget.ui ${GMAIL_PLUGIN_HEADERS} ${GMAIL_PLUGIN_SOURCES} gmail_plugin_ru_RU.ts)

target_sources(gmail-plugin PRIVATE  ${WRAP_MOC_GUI} ${UI_WRAP} ${QM_FILES})
set_target_properties(gmail-plugin PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(gmail-plugin  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/submodules/spdlog/include)
target_link_libraries(gmail-plugin PUBLIC plugin-api curl-wrapper persistent-storage nlohmann_json::nlohmann_json plugins-utils)
set_target_properties(gmail-plugin PROPERTIES
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON)

target_compile_options(gmail-plugin  PRIVATE -fPIC)
add_custom_command(TARGET gmail-plugin POST_BUILD
    COMMAND cmake -E make_directory ${PLUGINS_FOLDER}
    COMMAND cmake -E copy $<TARGET_FILE:gmail-plugin> ${PLUGINS_FOLDER}
    )

add_custom_command(TARGET gmail-plugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${QM_FILES}" "${LOCALE_FILES_DIR}")

