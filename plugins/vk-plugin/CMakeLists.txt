project(vk-plugin)
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)
find_package(Qt5LinguistTools REQUIRED)
find_package(nlohmann_json REQUIRED)

include(${TOP_DIR}/secrets.txt)
get_target_property(PLUGIN_API_SOURCE_DIR plugin-api SOURCE_DIR)
include(${PLUGIN_API_SOURCE_DIR}/version-defs.cmake)

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

    description.json.in
    )

file(MAKE_DIRECTORY  "${CMAKE_CURRENT_BINARY_DIR}/VK.plugin")
configure_file(description.json.in "${CMAKE_CURRENT_BINARY_DIR}/description.json.pred")
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/VK.plugin/description.json" INPUT "${CMAKE_CURRENT_BINARY_DIR}/description.json.pred" )
add_internationals(vk-plugin  ru_RU.UTF-8 "locale" "${CMAKE_CURRENT_BINARY_DIR}/VK.plugin/locale")

qt5_wrap_cpp(WRAP_MOC_GUI vkconnectionwidget.h)

qt5_wrap_ui(UI_WRAP vkconnectionwidget.ui)
qt5_create_translation(QM_FILES vkconnectionwidget.ui vkconnectionwidget.h vkconnectionwidget.cpp vk_plugin_ru_RU.ts)

target_sources(vk-plugin PRIVATE ${UI_WRAP} ${QM_FILES} ${WRAP_MOC_GUI})

target_include_directories(vk-plugin  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/submodules/spdlog/include)
target_link_libraries(vk-plugin PUBLIC plugin-api curl-wrapper nlohmann_json::nlohmann_json plugins-utils utils spdlog-headers)


set_target_properties(vk-plugin PROPERTIES
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON)


add_custom_command(TARGET vk-plugin POST_BUILD
    COMMAND cmake -E copy $<TARGET_FILE:vk-plugin> "${CMAKE_CURRENT_BINARY_DIR}/VK.plugin"
    COMMAND ${CMAKE_COMMAND} -E copy "${QM_FILES}" "${CMAKE_CURRENT_BINARY_DIR}/VK.plugin/locale"
)
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/VK.plugin" DESTINATION "plugins")
if(WIN32)
    target_compile_definitions(vk-plugin PRIVATE WIN_EXPORT)
endif()
