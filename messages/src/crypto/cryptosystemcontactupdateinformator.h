#ifndef CRYPTOSYSTEMCONTACTUPDATEINFORMATOR_H
#define CRYPTOSYSTEMCONTACTUPDATEINFORMATOR_H

#include <persistent-storage/watchers/enqueuedevents.h>
#include <memory>

class CryptoSystemImpl;
class Contact;

/**
 * @brief Отслеживает изменения в контейнере контактов, используется для
 * актуализации данных в CryptoSystemImpl
 */
class CryptoSystemContactUpdateInformator {
 public:
  explicit CryptoSystemContactUpdateInformator(
      std::shared_ptr<CryptoSystemImpl> system);

 public:
  void operator()(prstorage::EnqueuedEvents event, const Contact& contact);

 private:
  std::shared_ptr<CryptoSystemImpl> mSystem;
};

#endif  // CRYPTOSYSTEMCONTACTUPDATEINFORMATOR_H
