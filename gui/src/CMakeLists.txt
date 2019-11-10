project(gui)

find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets)
find_package (Threads)

set(GUI_MODULE_SOURCES
    basesettingsdialog.cpp
    widgets/contactwidget.cpp
    models/connectioninfomodel.cpp
    widgets/connectioninfowidget.cpp
    widgets/widgetsutils.cpp
    models/channelslistmodel.cpp
    dialogs/builders.cpp
    dialogs/contactgasket.cpp
    dialogs/connectioninfogasket.cpp
    models/dialogs-list/dialogusermodel.cpp
    wrappers/messagehandlerwrapper.cpp
    wrappers/dialogactionhandlerwrapper.cpp
    dialogs/userinformator.cpp
    dialogs/dialogcreation.cpp
    widgets/dialogactionmenu.cpp
    wrappers/dialoguserviewwrapper.cpp
    models/dialogs-list/dialoginfo.cpp
    models/dialogs-list/dialoginfodelegate.cpp
    models/dialogs-list/dialogsortmodel.cpp
    models/active-dialog-messages/usermessagemodeldelegate.cpp
    models/active-dialog-messages/activedialogmessagemodel.cpp
    dialogs/publickeydialog.cpp
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
    wrappers/messagehandlerwrapper.h
    wrappers/dialogactionhandlerwrapper.h
    dialogs/userinformator.h
    dialogs/dialogcreation.h
    widgets/dialogactionmenu.h
    wrappers/dialoguserviewwrapper.h
    models/dialogs-list/dialoginfo.h
    models/dialogs-list/dialoginfodelegate.h
    models/dialogs-list/dialogsortmodel.h
    models/active-dialog-messages/usermessagemodeldelegate.h
    models/active-dialog-messages/activedialogmessagemodel.h
    gui_helpers.h
    dialogs/publickeydialog.h
)

set(GUI_UI
    basesettingsdialog.ui
    widgets/contactwidget.ui
    widgets/connectioninfowidget.ui
    dialogs/dialogcreation.ui
    dialogs/publickeydialog.ui
)

set(
  GUI_MOC
  wrappers/messagehandlerwrapper.h
  wrappers/dialogactionhandlerwrapper.h
  wrappers/dialoguserviewwrapper.h
  basesettingsdialog.h
  widgets/dialogactionmenu.h
  widgets/connectioninfowidget.h
  widgets/contactwidget.h
  models/dialogs-list/dialogusermodel.h
  models/dialogs-list/dialoginfodelegate.h
  dialogs/connectioninfogasket.h
  dialogs/dialogcreation.h
  dialogs/userinformator.h
  dialogs/contactgasket.h
  models/connectioninfomodel.h
  dialogs/publickeydialog.h
)

qt5_wrap_cpp(WRAP_MOC_GUI ${GUI_MOC})

qt5_wrap_ui(UI_WRAP ${GUI_UI})


add_library(gui STATIC ${GUI_MODULE_SOURCES} ${GUI_MODULE_HEADERS} ${UI_WRAP} ${WRAP_MOC_GUI})

target_include_directories(gui PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries( gui messages Qt5::Core Qt5::Gui  Qt5::Widgets )
