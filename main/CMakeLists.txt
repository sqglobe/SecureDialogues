project(main)

find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)


# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})

set(MAIN_UI
    mainwindow.ui
)

set(MAIN_SOURCE
    main.cpp
    mainwindow.cpp
    initializer/chaneladapterfactory.cpp
    initializer/coreinitializer.cpp
    initializer/guiinitializer.cpp
    passask.cpp
)

set(MAIN_MOC
    mainwindow.h
    )

qt5_wrap_ui(UI_WRAP ${MAIN_UI})
qt5_wrap_cpp(MOC_WRAP ${MAIN_MOC})


add_executable(SecureDialogues ${UI_WRAP} ${MAIN_SOURCE} ${MOC_WRAP})
target_link_libraries(SecureDialogues Qt5::Core Qt5::Gui  Qt5::Widgets gui  )

if(MSVC)
  target_compile_options(SecureDialogues PRIVATE /W4 /WX)
else()
  target_compile_options(SecureDialogues PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set_property(TARGET SecureDialogues PROPERTY WIN32_EXECUTABLE true)
endif()
