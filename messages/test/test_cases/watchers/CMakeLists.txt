project(watchers)

add_executable(TestContactChangeWatcher testcontactchangewatcher.cpp)
add_test(NAME TestContactChangeWatcher COMMAND TestContactChangeWatcher)
target_link_libraries(TestContactChangeWatcher PRIVATE ${MY_TEST_LIBS} )

add_executable(TestDialogChangeWatcher testdialogchangewatcher.cpp)
add_test(NAME TestDialogChangeWatcher COMMAND TestDialogChangeWatcher)
target_link_libraries(TestDialogChangeWatcher PRIVATE ${MY_TEST_LIBS} )
