project(messages_unit_tests)

list(APPEND CMAKE_MODULE_PATH "${TOP_DIR}/submodules/cmake/FindBerkeleyDB")

add_subdirectory( src )

if (BUILD_TESTING)
  add_subdirectory( test)
endif()

