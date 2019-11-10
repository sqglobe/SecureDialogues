#include "publickeydialog.h"
#include "ui_publickeydialog.h"

#include "crypto/cryptosystemimpl.h"

#include "interfaces/abstractuserask.h"
#include "interfaces/abstractusernotifier.h"

PublicKeyDialog::PublicKeyDialog(std::shared_ptr<CryptoSystemImpl> system,
                                 std::shared_ptr<AbstractUserNotifier> notifier,
                                 std::shared_ptr<const AbstractUserAsk> ask,
                                 QWidget* parent) :
    QDialog(parent),
    ui(new Ui::PublicKeyDialog), mSystem(system), mNotifier(notifier),
    mAsk(ask) {
  ui->setupUi(this);

  connect(ui->generatePublickKey, &QPushButton::pressed, this,
          &PublicKeyDialog::generateKey);
}

PublicKeyDialog::~PublicKeyDialog() {
  delete ui;
}

void PublicKeyDialog::open() {
  ui->publicKeyValue->setPlainText(mSystem->exportPublicKey().c_str());
  QDialog::open();
}

void PublicKeyDialog::generateKey() {
  if (mAsk->ask(
          "Вы уверены, что хотите произвести генерацию ключей? В этом случае "
          "Вам придется уведомить все свои контакты о произошедшей смене")) {
    if (mSystem->generateAsymetricKeys()) {
      mNotifier->notify(
          AbstractUserNotifier::Severity::INFO,
          "Ключи для ассиметричного шифрования успешно сгенерированы");
    } else {
      mNotifier->notify(AbstractUserNotifier::Severity::INFO,
                        "Не удалось сгенерирвоать ключи для ассиметричного "
                        "шифрования. попытайтесь еще раз позже");
    }
  }
}
