#include "guiinitializer.h"

#include "coreinitializer.h"
#include "mainwindow.h"

#include "interfaces/abstractuserask.h"
#include "interfaces/abstractusernotifier.h"

#include "containers/dialogmanager.h"
#include "containers/messagecontainer.h"

#include "basesettingsdialog.h"
#include "dialogs/builders.h"
#include "dialogs/dialogcreation.h"

#include <QAction>
#include <QPushButton>
#include <QThread>

#include "wrappers/dialogactionhandlerwrapper.h"
#include "wrappers/messagehandlerwrapper.h"

#include <QListView>
#include "models/active-dialog-messages/activedialogmessagemodel.h"
#include "models/active-dialog-messages/usermessagemodeldelegate.h"
#include "models/dialogs-list/dialoginfodelegate.h"
#include "models/dialogs-list/dialogusermodel.h"
#include "widgets/dialogactionmenu.h"
#include "widgets/publickeyobject.h"
#include "wrappers/dialoguserviewwrapper.h"

/// dialogsViews
GuiInitializer::GuiInitializer(
    MainWindow* parent,
    const std::shared_ptr<CoreInitializer>& coreInit,
    const std::shared_ptr<AbstractUserAsk>& userAsk,
    const std::shared_ptr<AbstractUserNotifier>& userNotifier) :
    mChannelSettingsDialog(make_dialog(coreInit->getConnectionInfocontainer())),
    mContactsSettingsDialog(
        make_dialog(coreInit->getContactContainer(),
                    coreInit->getConnectionInfocontainer())),
    mDialogCreation(make_creation_dialog(coreInit->getContactContainer()))

{
  PublicKeyObject* pubObject = new PublicKeyObject(
      coreInit->getCryptoSystem(), userNotifier, userAsk, parent);
  QObject::connect(parent->findChild<QAction*>("actionChannels"),
                   &QAction::triggered, mChannelSettingsDialog.get(),
                   &BaseSettingsDialog::show);
  QObject::connect(parent->findChild<QAction*>("actionContacts"),
                   &QAction::triggered, mContactsSettingsDialog.get(),
                   &BaseSettingsDialog::show);

  QObject::connect(parent->findChild<QAction*>("generatePublicKeyAction"),
                   &QAction::triggered, pubObject,
                   &PublicKeyObject::generateNewKey);
  QObject::connect(parent->findChild<QAction*>("showPublicKeyAction"),
                   &QAction::triggered, pubObject,
                   &PublicKeyObject::showCurrentPublicKey);

  QThread* messageWorkThread = new QThread(parent);

  initMessageWrapper(coreInit, parent, userNotifier, messageWorkThread);
  initDialogWrapper(coreInit, parent, userNotifier, messageWorkThread);

  QObject::connect(parent, &MainWindow::closed, messageWorkThread,
                   &QThread::quit);

  messageWorkThread->start();

  auto messageModel = std::make_shared<ActiveDialogMessageModel>();

  coreInit->getMessageContainer()->registerHandler(messageModel);

  parent->findChild<QListView*>("activeMessages")->setModel(messageModel.get());
  auto activeMessageDelaget = new UserMessageModelDelegate(parent);
  parent->findChild<QListView*>("activeMessages")
      ->setItemDelegate(activeMessageDelaget);
}

void GuiInitializer::initMessageWrapper(
    const std::shared_ptr<CoreInitializer>& coreInit,
    MainWindow* parent,
    const std::shared_ptr<AbstractUserNotifier>&,
    QThread* messageThread) {
  // TODO Add actions to wrapers like send message, send dialog control command
  auto messageWraper =
      new MessageHandlerWrapper(coreInit->getMessageActionHandler());

  auto dialogUserModel =
      std::make_shared<DialogUserModel>(coreInit->getDialogManager());
  coreInit->getDialogManager()->registerWatcher(dialogUserModel);
  coreInit->getMessageContainer()->registerHandler(dialogUserModel);

  auto dialogsView = parent->findChild<QListView*>("dialogsViews");
  dialogsView->setContextMenuPolicy(Qt::CustomContextMenu);
  dialogsView->setItemDelegate(new DialogInfoDelegate(parent));

  mDialogUserViewWrapper = std::unique_ptr<DialogUserViewWrapper>(
      new DialogUserViewWrapper(dialogsView, dialogUserModel));
  messageWraper->moveToThread(messageThread);

  QObject::connect(dialogUserModel.get(),
                   &DialogUserModel::newDialogWasSelected, messageWraper,
                   &MessageHandlerWrapper::setActiveDialog);
  QObject::connect(dialogUserModel.get(),
                   &DialogUserModel::notValidDialogWasSelected, messageWraper,
                   &MessageHandlerWrapper::setActiveDialog);

  QObject::connect(dialogUserModel.get(),
                   &DialogUserModel::newDialogWasSelected, parent,
                   &MainWindow::on_goodDialogSelected);
  QObject::connect(dialogUserModel.get(),
                   &DialogUserModel::notValidDialogWasSelected, parent,
                   &MainWindow::on_badDialogSelected);

  QObject::connect(parent, &MainWindow::sendMessage, messageWraper,
                   &MessageHandlerWrapper::sendMessage);
  QObject::connect(dialogsView, &QListView::customContextMenuRequested,
                   mDialogUserViewWrapper.get(),
                   &DialogUserViewWrapper::requestToShowMenu);
}

void GuiInitializer::initDialogWrapper(
    const std::shared_ptr<CoreInitializer>& coreInit,
    MainWindow* parent,
    const std::shared_ptr<AbstractUserNotifier>& userNotifier,
    QThread* messageThread) {
  // TODO Add actions to wrapers like send message, send dialog control command
  auto dialogWraper =
      new DialogActionHandlerWrapper(coreInit->getDialogActionHandler());
  dialogWraper->moveToThread(messageThread);

  auto dialogMenu = new DialogActionMenu(userNotifier, parent);

  auto startDialog = parent->findChild<QPushButton*>("startNewDoalog");
  QObject::connect(startDialog, &QPushButton::clicked, mDialogCreation.get(),
                   &DialogCreation::show);
  QObject::connect(mDialogCreation.get(), &DialogCreation::createNewDialog,
                   dialogWraper,
                   &DialogActionHandlerWrapper::createDialogAction);
  QObject::connect(mDialogUserViewWrapper.get(),
                   &DialogUserViewWrapper::showMenuForDialog, dialogMenu,
                   &DialogActionMenu::showMenuAtPos);
  QObject::connect(dialogMenu, &DialogActionMenu::closeDialog, dialogWraper,
                   &DialogActionHandlerWrapper::closeDialogAction);
  QObject::connect(dialogMenu, &DialogActionMenu::acceptDialog, dialogWraper,
                   &DialogActionHandlerWrapper::acceptDialogAction);
  QObject::connect(dialogMenu, &DialogActionMenu::canceltDialog, dialogWraper,
                   &DialogActionHandlerWrapper::cancelDialogAction);
  QObject::connect(dialogMenu, &DialogActionMenu::removeDialog, dialogWraper,
                   &DialogActionHandlerWrapper::removeDialogAction);
}
