project(encryption_module)

add_subdirectory( src )

if (BUILD_TESTING)
  add_subdirectory( test )
endif()
