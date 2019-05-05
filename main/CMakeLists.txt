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



add_executable(SecureDialogues main.cpp mainwindow.cpp initializer/chaneladapterfactory.cpp initializer/coreinitializer.cpp initializer/guiinitializer.cpp passask.cpp)
target_link_libraries(SecureDialogues ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES}  ${Qt5Gui_LIBRARIES}   gui  )

