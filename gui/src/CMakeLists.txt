project(gui)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package (Threads)

set(GUI_MODULE_SOURCES
    basesettingsdialog.cpp
    widgets/contactwidget.cpp
    widgets/connectioninfowidget.cpp
    widgets/widgetsutils.cpp
    models/channelslistmodel.cpp
    dialogs/builders.cpp
    dialogs/contactgasket.cpp
    dialogs/connectioninfogasket.cpp
    models/dialogs-list/dialogusermodel.cpp
    models/activedialogmessagemodel.cpp
    wrappers/messagehandlerwrapper.cpp
    wrappers/dialogactionhandlerwrapper.cpp
    dialogs/userinformator.cpp
    dialogs/dialogcreation.cpp
    widgets/dialogactionmenu.cpp
    wrappers/dialoguserviewwrapper.cpp
    widgets/publickeyobject.cpp
    models/dialogs-list/dialoginfo.cpp
    models/dialogs-list/dialoginfodelegate.cpp
    models/dialogs-list/dialogsortmodel.cpp
)

set(GUI_MODULE_HEADERS
    basesettingsdialog.h
    dialogwidgetgasket.h
    models/containermodelgenerator.h
    models/contactmodel.h
    widgets/contactwidget.h
    models/connectioninfomodel.h
    widgets/connectioninfowidget.h
    widgets/correctnessinputerror.h
    widgets/widgetsutils.h
    models/channelslistmodel.h
    dialogs/builders.h
    dialogs/contactgasket.h
    dialogs/connectioninfogasket.h
    models/dialogs-list/dialogusermodel.h
    models/activedialogmessagemodel.h
    wrappers/messagehandlerwrapper.h
    wrappers/dialogactionhandlerwrapper.h
    dialogs/userinformator.h
    dialogs/dialogcreation.h
    widgets/dialogactionmenu.h
    wrappers/dialoguserviewwrapper.h
    widgets/publickeyobject.h
    models/dialogs-list/dialoginfo.h
    models/dialogs-list/dialoginfodelegate.h
    models/dialogs-list/dialogsortmodel.h
)


add_library(gui STATIC ${GUI_MODULE_SOURCES} ${GUI_MODULE_HEADERS})

target_include_directories(gui PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries( gui messages ${Qt5Widgets_LIBRARIES} )
