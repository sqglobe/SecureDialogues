project(messages-fakes)

FIND_PACKAGE(Qt5Core)

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Core_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Core_DEFINITIONS})

set(SOURCES fakefile.cpp fakeuserask.cpp)
set(HEADERS 
    fakechanneladapter.h
    fakechanneladaptersendmessageonly.h
    fakedialogmessage.h
    fakefile.h
    fakemessagecontainerhandler.h
    fakemessagedespather.h
    fakemessagehandler.h
    fakenotifier.h
    faketemplatechangewatcher.h
    fakeuserask.h
    cryptosystemfake.h
    despatcherrorsinkfake.h
)


add_library(messages-fakes ${SOURCES} ${HEADERS})
target_include_directories(messages-fakes PUBLIC   ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(messages-fakes messages ${Qt5Core_LIBRARIES})

