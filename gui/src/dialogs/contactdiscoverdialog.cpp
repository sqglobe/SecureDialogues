#include "contactdiscoverdialog.h"

#include <QMessageBox>
#include "communication/discovercontactagent.h"
#include "models/channelslistmodel.h"

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

  if (!is_address_valid(address.toUtf8().constData(), item.connectionType)) {
    QMessageBox::critical(this, tr("Error!"),
                          tr("Field 'Address' contains unsupported value"));
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
