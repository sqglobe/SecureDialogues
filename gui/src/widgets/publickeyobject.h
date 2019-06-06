#ifndef PUBLICKEYOBJECT_H
#define PUBLICKEYOBJECT_H

#include <QObject>
#include <memory>
class CryptoSystemImpl;
class AbstractUserAsk;
class AbstractUserNotifier;

/**
 * @brief Объект для выполнения действий с публичным ключом пользователя.
 * Позволяет отобразить ключ и выполнить повторную его генерацию.
 */
class PublicKeyObject : public QObject {
  Q_OBJECT
 public:
  PublicKeyObject(std::shared_ptr<CryptoSystemImpl> system,
                  std::shared_ptr<AbstractUserNotifier> notifier,
                  std::shared_ptr<const AbstractUserAsk> ask,
                  QObject* parent = nullptr);

 public slots:
  void generateNewKey();
  void showCurrentPublicKey();

 private:
  std::shared_ptr<CryptoSystemImpl> mSystem;
  std::shared_ptr<AbstractUserNotifier> mNotifier;
  std::shared_ptr<const AbstractUserAsk> mAsk;
};

#endif  // PUBLICKEYOBJECT_H
