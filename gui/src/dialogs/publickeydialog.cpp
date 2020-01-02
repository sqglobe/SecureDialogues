#include "publickeydialog.h"

#include "crypto/cryptosystemimpl.h"

#include <QMessageBox>

PublicKeyDialog::PublicKeyDialog(std::shared_ptr<CryptoSystemImpl> system,
                                 QWidget* parent) :
    TranslateChangeEventHandler<QDialog, Ui::PublicKeyDialog>(parent),
    ui(new Ui::PublicKeyDialog), mSystem(system) {
  ui->setupUi(this);

  connect(ui->generatePublickKey, &QPushButton::pressed, this,
          &PublicKeyDialog::generateKey);

  this->setUI(ui);
}

PublicKeyDialog::~PublicKeyDialog() {
  delete ui;
}

void PublicKeyDialog::generateKey() {
  const auto res = QMessageBox::question(
      this, tr("Choise required"),
      tr("Are you sure to generate new keys?"
         "Your must notify your contacts about new keys."),
      QMessageBox::Apply | QMessageBox::Cancel);
  if (QMessageBox::Apply == res) {
    if (mSystem->generateAsymetricKeys()) {
      QMessageBox::information(this, tr("Successful!"),
                               tr("Asymmetric keys successful generated"));
    } else {
      QMessageBox::information(
          this, tr("Error!"),
          tr("Failed to generate asymmetric keys. Please, try later"));
    }
  }
}

void PublicKeyDialog::showEvent(QShowEvent* event) {
  QDialog::showEvent(event);
  QCoreApplication::processEvents();
  ui->publicKeyValue->setPlainText(mSystem->exportPublicKey().c_str());
}
