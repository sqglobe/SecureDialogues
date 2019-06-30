project(tests)

find_package(Qt5Test REQUIRED)



SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(MY_TEST_LIBS Qt5::Test fakes )

ENABLE_TESTING()

target_link_libraries(messages)


add_subdirectory(fakes)



add_subdirectory( test_cases/containers )
add_subdirectory( test_cases/persistent_storage )
add_subdirectory( test_cases/messages )
add_subdirectory( test_cases/watchers )
add_subdirectory( test_cases/utils )
