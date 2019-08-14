#ifndef CRYPTOSYSTEMCONTACTUPDATEINFORMATOR_H
#define CRYPTOSYSTEMCONTACTUPDATEINFORMATOR_H

#include <memory>

#include "interfaces/changelistener.h"
#include "primitives/contact.h"

class CryptoSystemImpl;
class Contact;

/**
 * @brief Отслеживает изменения в контейнере контактов, используется для
 * актуализации данных в CryptoSystemImpl
 */
class CryptoSystemContactUpdateInformator : public ChangeListener<Contact> {
 public:
  explicit CryptoSystemContactUpdateInformator(
      std::shared_ptr<CryptoSystemImpl> system);

 public:
  void added(const element& element) override;
  void changed(const element& element) override;
  void removed(const element& element) override;

 private:
  std::shared_ptr<CryptoSystemImpl> mSystem;
};

#endif  // CRYPTOSYSTEMCONTACTUPDATEINFORMATOR_H
