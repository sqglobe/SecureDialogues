project(utils)

add_executable(TimeoutedRepositoryTest timeoutedrepositorytest.cpp)
add_test(NAME TimeoutedRepositoryTest COMMAND TimeoutedRepositoryTest)
target_link_libraries(TimeoutedRepositoryTest PRIVATE ${MY_TEST_LIBS} )
