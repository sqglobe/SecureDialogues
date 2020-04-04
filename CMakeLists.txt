cmake_minimum_required(VERSION 3.8)
project(SecureDialogues)

find_package(Git REQUIRED)


OPTION(
	USE_VMIME_SHARED
	"Use vmime as shared lib"
	ON
)


SET (PROJECT_OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/Product/SecureDialogues
        CACHE PATH
        "Directory for complete product"
    )

set(LOCALE_FILES_DIR "${PROJECT_BINARY_DIR}/bin/locale")
set(PLUGINS_FOLDER "${PROJECT_BINARY_DIR}/bin/plugins")
file(MAKE_DIRECTORY  "${LOCALE_FILES_DIR}")

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup used libraries
SET(JSON_BuildTests OFF)
SET(JSON_Install OFF)
SET(XG_TESTS OFF)


list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/submodules/cmake/FindBerkeleyDB" "${PROJECT_SOURCE_DIR}/cmake")

set(TOP_DIR ${PROJECT_SOURCE_DIR})
set(CROSSGUID_INCLUDE ${PROJECT_SOURCE_DIR}/submodules)

ENABLE_TESTING()

if(NOT EXISTS ${PROJECT_SOURCE_DIR}/submodules/cryptopp)
    message("cryptopp dir not exists. Make dirrectory")
    execute_process(
        COMMAND bash ../cryptopp.sh
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/submodules
        )
else()
    message("cryptopp dir exists")
endif()

IF (NOT USE_VMIME_SHARED)
  ADD_DEFINITIONS(-DVMIME_STATIC)
ENDIF()

add_library(spdlog-headers INTERFACE)
target_include_directories(spdlog-headers INTERFACE ${PROJECT_SOURCE_DIR}/submodules/spdlog/include)

add_library(eventpp-headers INTERFACE)
target_include_directories(eventpp-headers INTERFACE ${PROJECT_SOURCE_DIR}/submodules/eventpp/include)

add_library(boost-dll-headers INTERFACE)
target_include_directories(boost-dll-headers INTERFACE ${PROJECT_SOURCE_DIR}/submodules/boost-dll/include)

include(Gettext)

add_subdirectory(submodules/crossguid)
#add_subdirectory(submodules/cryptopp)
#add_subdirectory(submodules/nlohmann_json)
#add_subdirectory(submodules/fmt)

#add_library(json-hpp INTERFACE)
#target_include_directories(json-hpp INTERFACE ${PROJECT_SOURCE_DIR}/submodules/nlohmann_json/single_include/)
#add_subdirectory(submodules/json-schema-validator)

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


