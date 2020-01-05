project(encryption_test)

find_package(Qt5Test REQUIRED)
find_package (Threads)



SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(MY_TEST_LIBS Qt5::Test encryption ${CMAKE_THREAD_LIBS_INIT} cryptopp-static)

ENABLE_TESTING()

target_link_libraries(encryption)

add_subdirectory( test_cases/asymetrical )
add_subdirectory( test_cases/symetrical )
