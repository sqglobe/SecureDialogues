#include "contactdiscoverdialog.h"

#include <QMessageBox>
#include "communication/discovercontactagent.h"
#include "export/pluginaddressvalidator.h"
#include "models/channelslistmodel.h"
#include "support-functions.h"
Q_DECLARE_METATYPE(ChannelsListModel::ListItem);

ContactDiscoverDialog::ContactDiscoverDialog(
    std::shared_ptr<ChannelsListModel> model,
    std::shared_ptr<DiscoverContactAgent> agent,
    QWidget* parent) :
    TranslateChangeEventHandler<QDialog, Ui::ContactDiscoverDialog>(parent),
    ui(new Ui::ContactDiscoverDialog), mModel(std::move(model)),
    mAgent(std::move(agent)) {
  ui->setupUi(this);

  ui->connectionType->setModel(mModel.get());
  this->setUI(ui);
}

ContactDiscoverDialog::~ContactDiscoverDialog() {
  delete ui;
}

void ContactDiscoverDialog::on_sendContactBtn_clicked() {
  auto connInfoVariant = ui->connectionType->currentData();
  if (!connInfoVariant.canConvert<ChannelsListModel::ListItem>()) {
    QMessageBox::critical(this, tr("Error!"),
                          tr("Unacceptable value selected"));
    return;
  }

  auto item = qvariant_cast<ChannelsListModel::ListItem>(connInfoVariant);
  auto address = ui->adressValue->text();
  auto name = ui->name->text();

  if (address.isEmpty()) {
    QMessageBox::critical(this, tr("Fill the field!"),
                          tr("Empty field 'Address'"));
    return;
  }

  if (name.isEmpty()) {
    QMessageBox::critical(this, tr("Fill the field!"),
                          tr("Empty field 'Your name'"));
    return;
  }

  if (!item.validator) {
    QMessageBox::critical(
        this, tr("Error!"),
        tr("You select channel without plugin. Please place plugin into "
           "plugins folder and restart application"));
    return;
  }

  if (std::string errMess = plugin_support::make_string(
          item.validator->isValid(address.toUtf8().constData()));
      !errMess.empty()) {
    QMessageBox::critical(this, tr("Error!"), errMess.c_str());
    return;
  }

  try {
    mAgent->discover(name.toStdString(), address.toStdString(),
                     item.name.toStdString());
    QMessageBox::information(this, tr("Success"),
                             tr("Your contact data sended successfull"));
    this->hide();
  } catch (std::exception&) {
    QMessageBox::critical(
        this, tr("Error!"),
        tr("Error occured during message sending. Please, try later"));
  }
}

void ContactDiscoverDialog::showEvent(QShowEvent* event) {
  QDialog::showEvent(event);
  QCoreApplication::processEvents();
  ui->connectionType->setCurrentIndex(-1);
  ui->name->setText("");
  ui->adressValue->setText("");
}

void ContactDiscoverDialog::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    ui->retranslateUi(this);  // переведём окно заново
  } else {
    QDialog::changeEvent(event);
  }
}
