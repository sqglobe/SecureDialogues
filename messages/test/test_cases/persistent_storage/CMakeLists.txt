project(persistent_storage)

add_executable(ContactMarshallingTest contactmarshallingtest.cpp)
add_test(NAME ContactMarshallingTest COMMAND ContactMarshallingTest)
target_link_libraries(ContactMarshallingTest PRIVATE ${MY_TEST_LIBS} )
