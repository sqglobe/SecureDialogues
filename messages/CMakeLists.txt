project(messages_unit_tests)

add_subdirectory( src )

if (BUILD_TESTING)
  add_subdirectory( test)
endif()

