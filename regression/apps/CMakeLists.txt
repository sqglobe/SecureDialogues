project(apps)

find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)


# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})


    
add_executable(regression_with_one_dialog regression_with_one_dialog.cpp)
target_link_libraries(regression_with_one_dialog gui messages stand )


