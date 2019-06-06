project(gui_test)

find_package(Qt5Test REQUIRED)
find_package (Threads)



SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(MY_TEST_LIBS Qt5::Test gui messages fakes ${CMAKE_THREAD_LIBS_INIT})

ENABLE_TESTING()


add_executable(TestChannelListModel testchannellistmodel.cpp)
add_test(NAME TestChannelListModel COMMAND TestChannelListModel)
target_link_libraries(TestChannelListModel PRIVATE ${MY_TEST_LIBS})

add_executable(TestConnectionWidget testconnectionwidget.cpp fakeconnectioninfowidget.h)
add_test(NAME TestConnectionWidget COMMAND TestConnectionWidget)
target_link_libraries(TestConnectionWidget PRIVATE ${MY_TEST_LIBS})

add_executable(TestContactWidget testcontactwidget.cpp)
add_test(NAME TestContactWidget COMMAND TestContactWidget)
target_link_libraries(TestContactWidget PRIVATE ${MY_TEST_LIBS})
