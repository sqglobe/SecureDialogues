#include "contactdiscoverdialog.h"
#include "ui_contactdiscoverdialog.h"

#include "communication/discovercontactagent.h"
#include "interfaces/abstractusernotifier.h"
#include "models/channelslistmodel.h"

Q_DECLARE_METATYPE(ChannelsListModel::ListItem);

ContactDiscoverDialog::ContactDiscoverDialog(
    std::shared_ptr<ChannelsListModel> model,
    std::shared_ptr<DiscoverContactAgent> agent,
    std::shared_ptr<AbstractUserNotifier> notifier,
    QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ContactDiscoverDialog), mModel(std::move(model)),
    mAgent(std::move(agent)), mNotifier(std::move(notifier)) {
  ui->setupUi(this);

  ui->connectionType->setModel(mModel.get());
}

ContactDiscoverDialog::~ContactDiscoverDialog() {
  delete ui;
}

void ContactDiscoverDialog::on_sendContactBtn_clicked() {
  auto connInfoVariant = ui->connectionType->currentData();
  if (!connInfoVariant.canConvert<ChannelsListModel::ListItem>()) {
    mNotifier->notify(AbstractUserNotifier::Severity::ERROR,
                      "Выбрано не допустимое значение");
    return;
  }

  auto item = qvariant_cast<ChannelsListModel::ListItem>(connInfoVariant);
  auto address = ui->adressValue->text();
  auto name = ui->name->text();

  if (address.isEmpty()) {
    mNotifier->notify(AbstractUserNotifier::Severity::ERROR,
                      "Не заполнено поле 'Адресат'");
    return;
  }

  if (name.isEmpty()) {
    mNotifier->notify(AbstractUserNotifier::Severity::ERROR,
                      "Не заполнено поле 'Как представить'");
    return;
  }

  if (!is_address_valid(address.toUtf8().constData(), item.connectionType)) {
    mNotifier->notify(AbstractUserNotifier::Severity::ERROR,
                      "В поле 'Адресат' введено не допустимое значение");
    return;
  }

  try {
    mAgent->discover(name.toStdString(), address.toStdString(),
                     item.name.toStdString());
    mNotifier->notify(AbstractUserNotifier::Severity::SUCCESS,
                      "Ваши контактные данные отправлены успешно");
    this->hide();
  } catch (std::exception&) {
    mNotifier->notify(
        AbstractUserNotifier::Severity::ERROR,
        "Отправка сообщения завершилась с ошибкой, попытайтесь в другой раз");
  }
}

void ContactDiscoverDialog::showEvent(QShowEvent* event) {
  QDialog::showEvent(event);
  QCoreApplication::processEvents();
  ui->connectionType->setCurrentIndex(-1);
  ui->name->setText("");
  ui->adressValue->setText("");
}
