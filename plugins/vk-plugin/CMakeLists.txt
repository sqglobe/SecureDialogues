project(vk-plugin)
find_package(persistent-storage REQUIRED)
find_package(BerkeleyDB REQUIRED)
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)
find_package(Qt5LinguistTools REQUIRED)

include(${TOP_DIR}/secrets.txt)

add_library(vk-plugin MODULE
    vk-communication/base64.cpp
    vk-communication/base64.h
    vk-communication/vkapi.h
    vk-communication/vkapi.cpp
    vk-communication/vkoauth.h
    vk-communication/vkoauth.cpp
    vk-communication/oauth-exceptions.cpp
    vk-communication/oauth-exceptions.h
    vkaddressvalidator.cpp
    vkaddressvalidator.h
    vkconnectioninfo.h
    vkconnectioninfo.cpp
    vkconnectionserializer.h
    vkconnectionserializer.cpp
    vkconnectionwidget.h
    vkconnectionwidget.cpp
    vkmessagecommunicator.cpp
    vkmessagecommunicator.h
    vkfacade.h
    vkfacade.cpp
    vkplugindefs.h
    vkplugindefs.cpp
    )

qt5_wrap_cpp(WRAP_MOC_GUI vkconnectionwidget.h)

qt5_wrap_ui(UI_WRAP vkconnectionwidget.ui)
qt5_create_translation(QM_FILES vkconnectionwidget.ui vkconnectionwidget.h vkconnectionwidget.cpp vk_plugin_ru_RU.ts)

target_sources(vk-plugin PRIVATE ${UI_WRAP} ${QM_FILES} ${WRAP_MOC_GUI})

target_include_directories(vk-plugin  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/submodules/spdlog/include)
target_link_libraries(vk-plugin PUBLIC plugin-api curl-wrapper persistent-storage nlohmann_json::nlohmann_json plugins-utils)


set_target_properties(vk-plugin PROPERTIES
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON)

target_compile_options(vk-plugin  PRIVATE -fPIC)
add_custom_command(TARGET vk-plugin POST_BUILD
    COMMAND cmake -E make_directory ${PLUGINS_FOLDER}
    COMMAND cmake -E copy $<TARGET_FILE:vk-plugin> ${PLUGINS_FOLDER}
    )

add_custom_command(TARGET vk-plugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${QM_FILES}" "${LOCALE_FILES_DIR}")

add_internationals(vk-plugin  ru_RU.UTF-8 "locale" "${LOCALE_FILES_DIR}")
