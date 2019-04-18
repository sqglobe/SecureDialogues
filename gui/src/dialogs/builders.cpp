#include "builders.h"
#include "basesettingsdialog.h"
#include "containers/connectioninfocontainer.h"
#include "containers/contactcontainer.h"
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

#include "dialogcreation.h"

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
    const std::shared_ptr<ContactContainer>& contact,
    const std::shared_ptr<ConnectionInfoContainer>& connInfo) {
  auto contactModel = std::make_shared<ContactModel>(contact);
  contact->registerWatcher(contactModel);
  auto channelList =
      std::make_shared<ChannelsListModel>(connInfo->getAllElements());
  connInfo->registerTemporaryWatcher(channelList);

  auto widget = new ContactWidget(channelList);

  auto dialog =
      std::make_shared<BaseSettingsDialog>(contactModel.get(), widget);

  auto userInformator = std::make_shared<UserInformator>(dialog.get());

  auto gasket = new ContactGasket(
      std::unique_ptr<DialogWidgetGasket<ContactContainer, ContactWidget>>(
          new DialogWidgetGasket<ContactContainer, ContactWidget>(
              contact, widget, userInformator, userInformator)),
      dialog.get());

  contact_for_dialog(dialog, gasket, widget);

  return dialog;
}

std::shared_ptr<BaseSettingsDialog> make_dialog(
    const std::shared_ptr<ConnectionInfoContainer>& connInfo) {
  auto mainModel = std::make_shared<ConnectionInfoModel>(connInfo);
  connInfo->registerWatcher(mainModel);

  auto widget = new ConnectionInfoWidget();

  auto dialog = std::make_shared<BaseSettingsDialog>(mainModel.get(), widget);
  auto userInformator = std::make_shared<UserInformator>(dialog.get());

  auto gasket = new ConnectionInfoGasket(
      std::unique_ptr<
          DialogWidgetGasket<ConnectionInfoContainer, ConnectionInfoWidget>>(
          new DialogWidgetGasket<ConnectionInfoContainer, ConnectionInfoWidget>(
              connInfo, widget, userInformator, userInformator)),
      dialog.get());

  contact_for_dialog(dialog, gasket, widget);

  return dialog;
}

std::shared_ptr<DialogCreation> make_creation_dialog(
    const std::shared_ptr<ContactContainer>& contact) {
  auto contactModel = std::make_shared<ContactModel>(contact);
  contact->registerTemporaryWatcher(contactModel);
  auto dialog = std::make_shared<DialogCreation>(contactModel);
  return dialog;
}
