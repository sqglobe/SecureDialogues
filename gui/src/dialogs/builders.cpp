#include "builders.h"
#include "basesettingsdialog.h"
#include "dialogwidgetgasket.h"
#include "widgets/connectioninfowidget.h"
#include "widgets/contactwidget.h"

#include "dialogs/userinformator.h"
#include "models/channelslistmodel.h"
#include "models/connectioninfomodel.h"
#include "models/contactmodel.h"

#include "interfaces/abstractuserask.h"
#include "interfaces/abstractusernotifier.h"

#include "connectioninfogasket.h"
#include "contactgasket.h"

#include "communication/discovercontactagent.h"
#include "dialogcreation.h"
#include "dialogs/contactdiscoverdialog.h"
#include "dialogs/importdiscoveredcontactdialog.h"
#include "wrappers/recievedcontactsstoragewrapper.h"

#include <persistent-storage/watchers/enqueuedevents.h>
#include <persistent-storage/watchers/eventlistenerholder.h>

#include <QApplication>

//#define GENERATE_DIALOG_GASKET(GeneratedClassName, Container, Widget)

template <typename Gasket, typename Widget>
void contact_for_dialog(const std::shared_ptr<BaseSettingsDialog>& dialog,
                        Gasket* gasket,
                        Widget* widget) {
  QObject::connect(dialog.get(), &BaseSettingsDialog::actionAdd, gasket,
                   &Gasket::onActionAdd);
  QObject::connect(dialog.get(), &BaseSettingsDialog::actionRemoveAt, gasket,
                   &Gasket::onActionRemoveAt);
  QObject::connect(dialog.get(), &BaseSettingsDialog::actionUpdateAt, gasket,
                   &Gasket::onActionUpdateAt);
  QObject::connect(dialog.get(), &BaseSettingsDialog::actionViewAt, gasket,
                   &Gasket::onActionViewAt);

  QObject::connect(dialog.get(), &BaseSettingsDialog::actionCleare, widget,
                   &Widget::actionCleare);
  QObject::connect(dialog.get(), &BaseSettingsDialog::actionDisable, widget,
                   &Widget::actionDisable);
  QObject::connect(dialog.get(), &BaseSettingsDialog::actionEnable, widget,
                   &Widget::actionEnable);
}

std::shared_ptr<BaseSettingsDialog> make_dialog(
    const std::shared_ptr<ContactStorage>& contact,
    const std::shared_ptr<ConnectionStorage>& connInfo,
    const std::shared_ptr<Channel::EventQueue>& queue) {
  auto contactModel = std::make_shared<ContactModel>(contact->getAllElements());
  contact->appendPermanentListener(contactModel);

  auto channelList =
      std::make_shared<ChannelsListModel>(connInfo->getAllElements());
  auto channelEventListener = [channelList](
                                  Channel::ChannelStatus newStatus,
                                  const std::string& channelName,
                                  const std::string& message) mutable {
    channelList->updateChannelStatus(newStatus, channelName, message);
  };

  queue->appendListener(Channel::ChannelStatus::CONNECTED,
                        channelEventListener);
  queue->appendListener(Channel::ChannelStatus::FAILED_CONNECT,
                        channelEventListener);
  queue->appendListener(Channel::ChannelStatus::AUTHORIZATION_FAILED,
                        channelEventListener);

  connInfo->appendListener(channelList);

  auto widget = new ContactWidget(channelList);

  auto dialog = std::make_shared<BaseSettingsDialog>(contactModel, widget);

  auto userInformator = std::make_shared<UserInformator>(dialog.get());

  auto gasket = new ContactGasket(
      std::make_unique<DialogWidgetGasket<ContactStorage, ContactWidget>>(
          contact, widget, userInformator, userInformator),
      dialog.get());

  contact_for_dialog(dialog, gasket, widget);

  dialog->setWindowTitle(QApplication::tr("Add/edit contact"));

  return dialog;
}

std::shared_ptr<BaseSettingsDialog> make_dialog(
    const std::shared_ptr<ConnectionStorage>& connInfo,
    const std::shared_ptr<Channel::EventQueue>& queue) {
  auto mainModel =
      std::make_shared<ConnectionInfoModel>(connInfo->getAllElements());
  connInfo->appendPermanentListener(mainModel);

  auto channelEventListener = [mainModel](Channel::ChannelStatus newStatus,
                                          const std::string& channelName,
                                          const std::string& message) mutable {
    mainModel->updateChannelStatus(newStatus, channelName, message);
  };

  queue->appendListener(Channel::ChannelStatus::CONNECTED,
                        channelEventListener);
  queue->appendListener(Channel::ChannelStatus::FAILED_CONNECT,
                        channelEventListener);
  queue->appendListener(Channel::ChannelStatus::AUTHORIZATION_FAILED,
                        channelEventListener);

  auto widget = new ConnectionInfoWidget();

  auto connInfoWidgetListener = [widget](Channel::ChannelStatus newStatus,
                                         const std::string& channelName,
                                         const std::string& message) mutable {
    widget->updateChannelStatus(newStatus, channelName, message);
  };
  queue->appendListener(Channel::ChannelStatus::CONNECTED,
                        connInfoWidgetListener);
  queue->appendListener(Channel::ChannelStatus::FAILED_CONNECT,
                        connInfoWidgetListener);
  queue->appendListener(Channel::ChannelStatus::AUTHORIZATION_FAILED,
                        connInfoWidgetListener);

  auto dialog = std::make_shared<BaseSettingsDialog>(mainModel, widget);
  auto userInformator = std::make_shared<UserInformator>(dialog.get());

  auto gasket = new ConnectionInfoGasket(
      std::make_unique<
          DialogWidgetGasket<ConnectionStorage, ConnectionInfoWidget>>(
          connInfo, widget, userInformator, userInformator),
      dialog.get());

  contact_for_dialog(dialog, gasket, widget);

  dialog->setWindowTitle("Connection setup");

  return dialog;
}

std::shared_ptr<DialogCreation> make_creation_dialog(
    const std::shared_ptr<ContactStorage>& contact) {
  auto contactModel = std::make_shared<ContactModel>(contact->getAllElements());
  contact->appendListener(contactModel);
  auto dialog = std::make_shared<DialogCreation>(contactModel, contact);
  return dialog;
}

ContactDiscoverDialog* make_discover_dialog(
    const std::shared_ptr<ConnectionStorage>& connInfo,
    std::shared_ptr<DiscoverContactAgent> agent,
    std::shared_ptr<AbstractUserNotifier> notifier,
    const std::shared_ptr<Channel::EventQueue>& queue,
    QWidget* parent) {
  auto channelList =
      std::make_shared<ChannelsListModel>(connInfo->getAllElements());
  auto channelEventListener = [channelList](
                                  Channel::ChannelStatus newStatus,
                                  const std::string& channelName,
                                  const std::string& message) mutable {
    channelList->updateChannelStatus(newStatus, channelName, message);
  };

  queue->appendListener(Channel::ChannelStatus::CONNECTED,
                        channelEventListener);
  queue->appendListener(Channel::ChannelStatus::FAILED_CONNECT,
                        channelEventListener);
  queue->appendListener(Channel::ChannelStatus::AUTHORIZATION_FAILED,
                        channelEventListener);

  connInfo->appendListener(channelList);

  return new ContactDiscoverDialog(std::move(channelList), std::move(agent),
                                   parent);
}

ImportDiscoveredContactDialog* make_import_contact_dialog(
    std::shared_ptr<RecievedContactsStorageWrapper> wrapper,
    const std::shared_ptr<ConnectionStorage>& connInfo,
    const std::shared_ptr<Channel::EventQueue>& queue,
    QWidget* parent) {
  auto channelList =
      std::make_shared<ChannelsListModel>(connInfo->getAllElements());
  auto channelEventListener = [channelList](
                                  Channel::ChannelStatus newStatus,
                                  const std::string& channelName,
                                  const std::string& message) mutable {
    channelList->updateChannelStatus(newStatus, channelName, message);
  };

  queue->appendListener(Channel::ChannelStatus::CONNECTED,
                        channelEventListener);
  queue->appendListener(Channel::ChannelStatus::FAILED_CONNECT,
                        channelEventListener);
  queue->appendListener(Channel::ChannelStatus::AUTHORIZATION_FAILED,
                        channelEventListener);

  connInfo->appendListener(channelList);

  auto widget = std::make_shared<ContactWidget>(channelList);

  return new ImportDiscoveredContactDialog(std::move(widget),
                                           std::move(wrapper), parent);
}
