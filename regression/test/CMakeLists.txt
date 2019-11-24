find_package(Qt5Test REQUIRED)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(MY_TEST_LIBS Qt5::Test stand )

add_executable(QueuedActionsChannelAdapterTest queuedactionschanneladaptertest.cpp)
add_test(NAME QueuedActionsChannelAdapterTest COMMAND QueuedActionsChannelAdapterTest)
target_link_libraries(QueuedActionsChannelAdapterTest PRIVATE ${MY_TEST_LIBS} )

#successdialogcreationtest

add_executable(SuccessDialogCreationTest successdialogcreationtest.cpp)
add_test(NAME SuccessDialogCreationTest COMMAND SuccessDialogCreationTest)
target_link_libraries(SuccessDialogCreationTest PRIVATE ${MY_TEST_LIBS} )
