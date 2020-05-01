project(smtp-plugin)
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)
find_package(Qt5LinguistTools REQUIRED)
find_package(vmime REQUIRED)

get_target_property(PLUGIN_API_SOURCE_DIR plugin-api SOURCE_DIR)
include(${PLUGIN_API_SOURCE_DIR}/version-defs.cmake)


add_library(smtp-plugin MODULE
    email-agent/imapreciever.cpp
    email-agent/imapreciever.h
    email-agent/skipcertverifier.cpp
    email-agent/skipcertverifier.h
    email-agent/smtpsender.cpp
    email-agent/smtpsender.h
    smtpaddressvalidator.h
    smtpaddressvalidator.cpp
    smtpconnectioninfo.h
    smtpconnectioninfo.cpp
    smtpplugindefs.cpp
    smtpplugindefs.h
    smtpconnectionserializer.h
    smtpconnectionserializer.cpp
    smtpconnectionwidget.cpp
    smtpconnectionwidget.h
    smtpmessagecommunication.h
    smtpmessagecommunication.cpp
    smtpfacade.h
    smtpfacade.cpp

    description.json.in
)

file(MAKE_DIRECTORY  "${CMAKE_CURRENT_BINARY_DIR}/Email.plugin")
configure_file(description.json.in "${CMAKE_CURRENT_BINARY_DIR}/description.json.pred")
file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Email.plugin/description.json" INPUT "${CMAKE_CURRENT_BINARY_DIR}/description.json.pred" )
add_internationals(smtp-plugin  ru_RU.UTF-8 "locale" "${CMAKE_CURRENT_BINARY_DIR}/Email.plugin/locale")


qt5_wrap_cpp(WRAP_MOC_GUI smtpconnectionwidget.h)

qt5_wrap_ui(UI_WRAP smtpconnectionwidget.ui)
qt5_create_translation(QM_FILES smtpconnectionwidget.ui smtpconnectionwidget.h smtpconnectionwidget.cpp smtp_plugin_ru_RU.ts)

target_sources(smtp-plugin PRIVATE ${UI_WRAP} ${QM_FILES} ${WRAP_MOC_GUI})

target_include_directories(smtp-plugin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(smtp-plugin PRIVATE plugin-api plugins-utils utils spdlog-headers  Qt5::Core Qt5::Gui  Qt5::Widgets CompilationFlags vmime)

if(WIN32)
    target_link_libraries(smtp-plugin PRIVATE intl)
    target_compile_definitions(smtp-plugin PRIVATE WIN_EXPORT)
endif()

if(USE_VMIME_SHARED)
    file(MAKE_DIRECTORY  "${CMAKE_CURRENT_BINARY_DIR}/Email.plugin/libs")
    add_custom_command(TARGET smtp-plugin POST_BUILD
        COMMAND cmake -E copy $<TARGET_FILE:vmime> "${CMAKE_CURRENT_BINARY_DIR}/Email.plugin/libs/"
    )
   target_link_libraries(smtp-plugin PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wl,-rpath,\$ORIGIN/libs/>)
endif()

set_target_properties(smtp-plugin PROPERTIES
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON)

enable_interprocedural_optimization(smtp-plugin)

add_custom_command(TARGET smtp-plugin POST_BUILD
    COMMAND cmake -E copy $<TARGET_FILE:smtp-plugin> "${CMAKE_CURRENT_BINARY_DIR}/Email.plugin"
    COMMAND ${CMAKE_COMMAND} -E copy "${QM_FILES}" "${CMAKE_CURRENT_BINARY_DIR}/Email.plugin/locale"
)


install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Email.plugin" DESTINATION "plugins")
