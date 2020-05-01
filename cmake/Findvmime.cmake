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
if(USE_VMIME_SHARED)
    add_library(vmime SHARED IMPORTED)
else()
    add_library(vmime STATIC IMPORTED)
    find_package(GnuTLS REQUIRED)
    target_link_libraries(vmime INTERFACE gsasl ${GNUTLS_LIBRARIES})
    target_compile_definitions(vmime INTERFACE VMIME_STATIC)
    if(NOT WIN32)
        target_link_libraries(vmime INTERFACE anl)
    endif()
endif()

if(WIN32)
    target_link_libraries(vmime INTERFACE ws2_32 iconv)
endif()

if(NOT Vmime_FOUND)
    message(FATAL_ERROR "Cant file vmime")
endif()

if(IS_SYMLINK "${VMIME_LIBRARY}")
  file(READ_SYMLINK "${VMIME_LIBRARY}" VMIME_LINK_RES)
  if(NOT IS_ABSOLUTE "${VMIME_LINK_RES}")
    get_filename_component(dir "${VMIME_LIBRARY}" DIRECTORY)
    set(VMIME_LIBRARY "${dir}/${VMIME_LINK_RES}")
  else()
    set(VMIME_LIBRARY "${VMIME_LINK_RES}")
  endif()
endif()

set_property(TARGET vmime PROPERTY IMPORTED_LOCATION ${VMIME_LIBRARY})
target_include_directories(vmime INTERFACE "${VMIME_INCLUDE_DIRS}")
message("VMIME found ${VMIME_LIBRARY}")
