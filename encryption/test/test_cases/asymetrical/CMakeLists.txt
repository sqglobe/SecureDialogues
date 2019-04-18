project(asymetrical)


add_executable(RemoteOperationsTest tst_remotepeeroperationstest.cpp)
add_test(NAME RemoteOperationsTest COMMAND RemoteOperationsTest)
target_link_libraries(RemoteOperationsTest PRIVATE ${MY_TEST_LIBS} )

add_executable(LocalOperationsTest tst_localpeeroperationstest.cpp)
add_test(NAME LocalOperationsTest COMMAND LocalOperationsTest)
target_link_libraries(LocalOperationsTest PRIVATE ${MY_TEST_LIBS} )
