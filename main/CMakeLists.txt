project(main)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)


# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
#set(QT5_PROJECT_LIBRARIES "")
if(WIN32)
    message("Used for windows build")
    find_package(PkgConfig)
    add_definitions(-DQT_STATICPLUGIN)
    pkg_check_modules(QT5ALL REQUIRED Qt5Widgets Qt5Gui Qt5Widgets)
    list(APPEND QT5_PROJECT_LIBRARIES Qt5::QWindowsIntegrationPlugin Qt5::QICOPlugin ${QT5ALL_LDFLAGS} ${PKGDEPS_STATIC_LDFLAGS} -static)
    add_definitions(-DWINDOWS_SECURE_DIALOGUES)
    SET(CMAKE_EXE_LINKER_FLAGS  "-mwindows")
    link_directories(${PKGDEPS_STATIC_LIBRARY_DIRS})
endif()


add_executable(SecureDialogues main.cpp mainwindow.cpp initializer/chaneladapterfactory.cpp initializer/coreinitializer.cpp initializer/guiinitializer.cpp passask.cpp)
target_link_libraries(SecureDialogues ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES}  ${Qt5Gui_LIBRARIES}   gui ${QT5_PROJECT_LIBRARIES} )

