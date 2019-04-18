project(symetrical)


add_executable(TotalAesSystemTest tst_totalaessystemtest.cpp)
add_test(NAME TotalAesSystemTest COMMAND TotalAesSystemTest)
target_link_libraries(TotalAesSystemTest PRIVATE ${MY_TEST_LIBS} )

