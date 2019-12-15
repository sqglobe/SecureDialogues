#include "guiinitializer.h"

#include "interfaces/abstractcoreinitializer.h"
#include "mainwindow.h"

#include "interfaces/abstractuserask.h"
#include "interfaces/abstractusernotifier.h"

#include <QCommandLinkButton>
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
#include "containers/storages.h"
#include "dialogs/contactdiscoverdialog.h"
#include "dialogs/publickeydialog.h"
#include "dialogs/recieveddiscoveredcontactsdialog.h"
#include "models/active-dialog-messages/activedialogmessagemodel.h"
#include "models/active-dialog-messages/usermessagemodeldelegate.h"
#include "models/dialogs-list/dialoginfodelegate.h"
#include "models/dialogs-list/dialogusermodel.h"
#include "models/discovered-contacts/discoveredcontactmodel.h"
#include "widgets/dialogactionmenu.h"
#include "widgets/recievedcontactsmenu.h"
#include "wrappers/dialoguserviewwrapper.h"
#include "wrappers/recievedcontactsstoragewrapper.h"

/// dialogsViews
GuiInitializer::GuiInitializer(
    MainWindow* parent,
    const std::shared_ptr<AbstractCoreInitializer>& coreInit,
    const std::shared_ptr<AbstractUserAsk>& userAsk,
    const std::shared_ptr<AbstractUserNotifier>& userNotifier,
    const std::shared_ptr<Channel::EventQueue>& eventQueue) :
    mChannelSettingsDialog(
        make_dialog(coreInit->getConnectionStorage(), eventQueue)),
    mContactsSettingsDialog(make_dialog(coreInit->getContactStorage(),
                                        coreInit->getConnectionStorage(),
                                        eventQueue)),
    mDialogCreation(make_creation_dialog(coreInit->getContactStorage())),
    mDiscoveredContactModel(std::make_shared<DiscoveredContactModel>(
        coreInit->getDiscoveredContactStorage()->getAllElements())),
    mRecievedContactsStorageWrapper(
        std::make_shared<RecievedContactsStorageWrapper>(
            coreInit->getDiscoveredContactStorage(),
            coreInit->getContactStorage()))

{
  coreInit->getDiscoveredContactStorage()->appendPermanentListener(
      mDiscoveredContactModel);
  initSimpleDialogs(parent, coreInit, userAsk, userNotifier, eventQueue);

  auto messageWorkThread = new QThread(parent);

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
    const std::shared_ptr<AbstractCoreInitializer>& coreInit,
    MainWindow* parent,
    const std::shared_ptr<AbstractUserNotifier>&,
    QThread* messageThread) {
  // TODO Add actions to wrapers like send message, send dialog control command
  auto messageWraper =
      new MessageHandlerWrapper(coreInit->getMessageActionHandler());

  auto dialogUserModel = std::make_shared<DialogUserModel>(
      coreInit->getContactStorage(),
      coreInit->getDialogStorage()->getAllElements());
  coreInit->getDialogStorage()->appendPermanentListener(dialogUserModel);
  coreInit->getMessageContainer()->registerHandler(dialogUserModel);

  auto dialogsView = parent->findChild<QListView*>("dialogsViews");
  dialogsView->setContextMenuPolicy(Qt::CustomContextMenu);
  dialogsView->setItemDelegate(new DialogInfoDelegate(parent));

  mDialogUserViewWrapper =
      std::make_unique<DialogUserViewWrapper>(dialogsView, dialogUserModel);
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
    const std::shared_ptr<AbstractCoreInitializer>& coreInit,
    MainWindow* parent,
    const std::shared_ptr<AbstractUserNotifier>& userNotifier,
    QThread* messageThread) {
  // TODO Add actions to wrapers like send message, send dialog control command
  auto dialogWraper = new DialogActionHandlerWrapper(
      coreInit->getDialogActionHandler(), userNotifier);
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
  QObject::connect(dialogMenu, &DialogActionMenu::removeDialog, dialogWraper,
                   &DialogActionHandlerWrapper::removeDialogAction);
}

void GuiInitializer::initSimpleDialogs(
    MainWindow* parent,
    const std::shared_ptr<AbstractCoreInitializer>& coreInit,
    const std::shared_ptr<AbstractUserAsk>& userAsk,
    const std::shared_ptr<AbstractUserNotifier>& userNotifier,
    const std::shared_ptr<Channel::EventQueue>& eventQueue) {
  auto* pubDialog = new PublicKeyDialog(coreInit->getCryptoSystem(),
                                        userNotifier, userAsk, parent);
  QObject::connect(parent->findChild<QCommandLinkButton*>("connectionButton"),
                   &QCommandLinkButton::pressed, mChannelSettingsDialog.get(),
                   &BaseSettingsDialog::show);
  QObject::connect(parent->findChild<QCommandLinkButton*>("contactBurtton"),
                   &QCommandLinkButton::pressed, mContactsSettingsDialog.get(),
                   &BaseSettingsDialog::show);

  QObject::connect(parent->findChild<QCommandLinkButton*>("publicKeyButton"),
                   &QCommandLinkButton::pressed, pubDialog,
                   &PublicKeyDialog::show);

  auto discoverDialog = make_discover_dialog(
      coreInit->getConnectionStorage(), coreInit->getDiscoverContactAgent(),
      userNotifier, eventQueue, parent);
  QObject::connect(parent->findChild<QPushButton*>("makeContact"),
                   &QCommandLinkButton::pressed, discoverDialog,
                   &PublicKeyDialog::show);

  auto* recievedDialog = new RecievedDiscoveredContactsDialog(
      mDiscoveredContactModel.get(), mRecievedContactsStorageWrapper, parent);

  QObject::connect(
      parent->findChild<QCommandLinkButton*>("openRecievedContacts"),
      &QCommandLinkButton::pressed, recievedDialog, &BaseSettingsDialog::show);

  auto* menu = new RecievedContactsMenu(recievedDialog);

  QObject::connect(recievedDialog,
                   &RecievedDiscoveredContactsDialog::showDialogMenu, menu,
                   &RecievedContactsMenu::showMenuAtPos);
  QObject::connect(menu, &RecievedContactsMenu::removeRecievedContact,
                   recievedDialog,
                   &RecievedDiscoveredContactsDialog::removeRecievedContact);
}
