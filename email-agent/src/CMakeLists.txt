project(email-agent)

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
message("VMIME_LIBRARY ${VMIME_LIBRARY}, VMIME_INCLUDE_DIR ${VMIME_INCLUDE_DIR}")
set(EMAIL_WIN_LIBS "")

if(WIN32)
    list(APPEND EMAIL_WIN_LIBS ws2_32 iconv)
endif()

if(NOT USE_VMIME_SHARED)
    find_package(GnuTLS REQUIRED)
    list(APPEND EMAIL_WIN_LIBS gsasl ${GNUTLS_LIBRARIES})
endif()

if(NOT USE_VMIME_SHARED AND NOT WIN32)
  list(APPEND EMAIL_WIN_LIBS anl)
endif()


set( EMAIL_SRC
     email-agent/imapreciever.cpp
     email-agent/skipcertverifier.cpp
     email-agent/smtpsender.cpp
)

set( EMAIL_HEADER
    email-agent/imapreciever.h
    email-agent/skipcertverifier.h
    email-agent/smtpsender.h
    )

add_library(email-agent STATIC ${EMAIL_SRC} ${EMAIL_HEADER})

target_include_directories(email-agent PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${MCRYPTO_INCLUDE_DIR} ${VMIME_INCLUDE_DIRS})
target_link_libraries(email-agent PUBLIC  ${VMIME_LIBRARIES} ${EMAIL_WIN_LIBS})


