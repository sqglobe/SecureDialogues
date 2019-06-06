#include "publickeyobject.h"
#include <QInputDialog>
#include "crypto/cryptosystemimpl.h"

#include "interfaces/abstractuserask.h"
#include "interfaces/abstractusernotifier.h"

PublicKeyObject::PublicKeyObject(std::shared_ptr<CryptoSystemImpl> system,
                                 std::shared_ptr<AbstractUserNotifier> notifier,
                                 std::shared_ptr<const AbstractUserAsk> ask,
                                 QObject* parent) :
    QObject(parent),
    mSystem(std::move(system)), mNotifier(std::move(notifier)),
    mAsk(std::move(ask)) {}

void PublicKeyObject::generateNewKey() {
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

void PublicKeyObject::showCurrentPublicKey() {
  QInputDialog dialog;

  dialog.setInputMode(QInputDialog::TextInput);
  dialog.setWindowTitle("Публичный ключ");
  dialog.setOptions(QInputDialog::UsePlainTextEditForTextInput);
  dialog.setLabelText("Скопируйте значение публичного ключа");
  dialog.setTextValue(mSystem->exportPublicKey().c_str());
  dialog.exec();
}
