cmake_minimum_required(VERSION 3.8)
project(SecureDialogues)

find_package(Git REQUIRED)


OPTION(
	USE_VMIME_SHARED
	"Use vmime as shared lib"
	ON
)

set(LOCALE_FILES_DIR "${PROJECT_BINARY_DIR}/bin/locale")
set(PLUGINS_FOLDER "${PROJECT_BINARY_DIR}/bin/plugins")
file(MAKE_DIRECTORY  "${LOCALE_FILES_DIR}")

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(XG_TESTS OFF)


list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/submodules/cmake/FindBerkeleyDB" "${PROJECT_SOURCE_DIR}/cmake")

include(compilation-flags.target)

set(TOP_DIR ${PROJECT_SOURCE_DIR})

ENABLE_TESTING()

IF (NOT USE_VMIME_SHARED)
  ADD_DEFINITIONS(-DVMIME_STATIC)
ENDIF()

add_library(spdlog-headers INTERFACE)
target_include_directories(spdlog-headers INTERFACE ${PROJECT_SOURCE_DIR}/submodules/spdlog/include)

add_library(eventpp-headers INTERFACE)
target_include_directories(eventpp-headers INTERFACE ${PROJECT_SOURCE_DIR}/submodules/eventpp/include)

add_library(dynalo-headers INTERFACE)
target_include_directories(dynalo-headers INTERFACE ${PROJECT_SOURCE_DIR}/submodules/dynalo/include)



add_library(crossguid-headers INTERFACE)
target_include_directories(crossguid-headers INTERFACE ${PROJECT_SOURCE_DIR}/submodules)

include(Gettext)

add_subdirectory(submodules/crossguid)

add_subdirectory( messages )
add_subdirectory( gui )
add_subdirectory( main )
add_subdirectory(encryption)
add_subdirectory(utils)
add_subdirectory(doc)
add_subdirectory(plugin-api)
add_subdirectory(plugins-support )

add_subdirectory(plugins/gmail-plugin)
add_subdirectory(plugins/vk-plugin)
add_subdirectory(plugins/smtp-plugin)
add_subdirectory(plugins/plugins-libs/plugin-utils)
add_subdirectory(plugins/plugins-libs/curl-wrapper)

if (BUILD_TESTING)
  add_subdirectory( regression )
endif()


