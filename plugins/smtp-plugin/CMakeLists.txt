project(smtp-plugin)
find_package(persistent-storage REQUIRED)
find_package(BerkeleyDB REQUIRED)
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)
find_package(Qt5LinguistTools REQUIRED)

if (NOT WIN32 AND NOT  VMIME_ROOT)
   # try using pkg-config to get the directories and then use these values
   # in the find_path() and find_library() calls
   # also fills in VMIME_DEFINITIONS, although that isn't normally useful
   find_package(PkgConfig QUIET)
   PKG_CHECK_MODULES(PC_VMIME QUIET vmime)
   set(VMIME_DEFINITIONS ${PC_VMIME_CFLAGS_OTHER})
   set(VMIME_VERSION_STRING ${PC_VMIME_VERSION})
endif ()

find_path(VMIME_INCLUDE_DIR vmime/vmime.hpp
   HINTS
   ${VMIME_ROOT}/include/
   ${PC_VMIME_INCLUDEDIR}
   ${PC_VMIME_INCLUDE_DIRS}
   )

find_library(VMIME_LIBRARY NAMES vmime
   HINTS
   ${VMIME_ROOT}/lib/
   ${PC_VMIME_LIBDIR}
   ${PC_VMIME_LIBRARY_DIRS}
   )

mark_as_advanced(VMIME_INCLUDE_DIR VMIME_LIBRARY)

# handle the QUIETLY and REQUIRED arguments and set Vmime_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Vmime
                                  REQUIRED_VARS VMIME_LIBRARY VMIME_INCLUDE_DIR
                                  VERSION_VAR VMIME_VERSION_STRING)

if(Vmime_FOUND)
    set(VMIME_LIBRARIES    ${VMIME_LIBRARY})
    set(VMIME_INCLUDE_DIRS ${VMIME_INCLUDE_DIR})
else()
    message(FATAL_ERROR "Cant file vmime")
endif()

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
)

add_internationals(smtp-plugin  ru_RU.UTF-8 "locale" "${LOCALE_FILES_DIR}")

qt5_wrap_cpp(WRAP_MOC_GUI smtpconnectionwidget.h)

qt5_wrap_ui(UI_WRAP smtpconnectionwidget.ui)
qt5_create_translation(QM_FILES smtpconnectionwidget.ui smtpconnectionwidget.h smtpconnectionwidget.cpp smtp_plugin_ru_RU.ts)

target_sources(smtp-plugin PRIVATE ${UI_WRAP} ${QM_FILES} ${WRAP_MOC_GUI})

set_target_properties(smtp-plugin PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(smtp-plugin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${VMIME_INCLUDE_DIRS})
target_link_libraries(smtp-plugin PUBLIC  ${VMIME_LIBRARIES} plugin-api persistent-storage plugins-utils)

if(WIN32)
    target_link_libraries(smtp-plugin PRIVATE ws2_32 iconv)
endif()

if(NOT USE_VMIME_SHARED)
    find_package(GnuTLS REQUIRED)
    target_link_libraries(smtp-plugin PRIVATE gsasl ${GNUTLS_LIBRARIES})
endif()

if(NOT USE_VMIME_SHARED AND NOT WIN32)
    target_link_libraries(smtp-plugin PRIVATE anl)
endif()

set_target_properties(smtp-plugin PROPERTIES
    PREFIX ""
    POSITION_INDEPENDENT_CODE ON)

target_compile_options(smtp-plugin  PRIVATE -fPIC)
add_custom_command(TARGET smtp-plugin POST_BUILD
    COMMAND cmake -E make_directory ${PLUGINS_FOLDER}
    COMMAND cmake -E copy $<TARGET_FILE:smtp-plugin> ${PLUGINS_FOLDER}
    )

add_custom_command(TARGET smtp-plugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${QM_FILES}" "${LOCALE_FILES_DIR}")


