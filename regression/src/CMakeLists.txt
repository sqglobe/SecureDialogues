project(stand)

find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)


# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})


set(SOURCES
    stand-helpers/queuedactionschanneladapter.cpp
    stand/dialogcreationandmessagesexchangecorestand.cpp
    )
set(HEADERS
    stand-helpers/queuedactionschanneladapter.h
    stand/dialogcreationandmessagesexchangecorestand.h
    )


add_library(stand STATIC ${SOURCES} ${HEADERS})
target_include_directories(stand PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(stand Qt5::Core Qt5::Gui  Qt5::Widgets gui messages-fakes)
