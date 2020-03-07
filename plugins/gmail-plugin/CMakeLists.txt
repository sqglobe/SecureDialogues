project(gmail-plugin)
find_package(persistent-storage REQUIRED)
find_package(BerkeleyDB REQUIRED)
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)

include(${TOP_DIR}/secrets.txt)

SET(GMAIL_PLUGIN_SOURCES
    gmailplugindefs.cpp
    gmailconnectioninfo.cpp
    gmailconnectionserializer.cpp
    gmailconnectionwidget.cpp
    gmailrecievedmessagesiterator.cpp
    gmailmessagecommunicator.cpp
    gmailfacade.cpp
    gmail-communication/gmailapi.cpp
    gmail-communication/gmailoauth.cpp
    gmail-communication/base64.cpp
    gmailaddressvalidator.h
    )

SET(GMAIL_PLUGIN_HEADERS
    gmailconnectioninfo.h
    gmailplugindefs.h
    gmailconnectionserializer.h
    gmailconnectionwidget.h
    gmailrecievedmessagesiterator.h
    gmailmessagecommunicator.h
    gmailfacade.h
    gmail-communication/gmailapi.h
    gmail-communication/gmailoauth.h
    gmail-communication/gmailexceptions.h
    gmail-communication/base64.h
    gmailaddressvalidator.cpp
    )

qt5_wrap_cpp(WRAP_MOC_GUI gmailconnectionwidget.h)

qt5_wrap_ui(UI_WRAP gmailconnectionwidget.ui)

add_library(gmail-plugin MODULE ${GMAIL_PLUGIN_SOURCES} ${GMAIL_PLUGIN_HEADERS} ${WRAP_MOC_GUI} ${UI_WRAP})
set_target_properties(gmail-plugin PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(gmail-plugin  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/submodules/spdlog/include)
target_link_libraries(gmail-plugin PUBLIC plugin-api curl-wrapper persistent-storage nlohmann_json::nlohmann_json )
set_target_properties(gmail-plugin PROPERTIES
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON)

add_compile_options(-fPIC)
add_custom_command(TARGET gmail-plugin POST_BUILD
    COMMAND cmake -E make_directory ${PLUGINS_FOLDER}
    COMMAND cmake -E copy $<TARGET_FILE:gmail-plugin> ${PLUGINS_FOLDER}
    )
