project(gui_test)

find_package(Qt5Test REQUIRED)
find_package (Threads)
find_package(BerkeleyDB REQUIRED)
find_package(persistent-storage REQUIRED)
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)


SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(MY_TEST_LIBS Qt5::Test gui messages messages-fakes utils curl-wrapper
    ${CMAKE_THREAD_LIBS_INIT} persistent-storage  Qt5::Core Qt5::Gui  Qt5::Widgets xg)

ENABLE_TESTING()

