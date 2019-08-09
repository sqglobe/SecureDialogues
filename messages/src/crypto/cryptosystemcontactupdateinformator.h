#ifndef CRYPTOSYSTEMCONTACTUPDATEINFORMATOR_H
#define CRYPTOSYSTEMCONTACTUPDATEINFORMATOR_H

#include <memory>
#include "interfaces/changewatcher.h"

class CryptoSystemImpl;

/**
 * @brief Отслеживает изменения в контейнере контактов, используется для
 * актуализации данных в CryptoSystemImpl
 */
class CryptoSystemContactUpdateInformator
    : public ChangeWatcher<ContactContainer::const_element> {
 public:
  explicit CryptoSystemContactUpdateInformator(
      std::shared_ptr<CryptoSystemImpl> system);

 public:
  void added(const element& obj) override;
  void changed(const element& obj) override;
  void removed(const element& obj) override;

 private:
  std::shared_ptr<CryptoSystemImpl> mSystem;
};

#endif  // CRYPTOSYSTEMCONTACTUPDATEINFORMATOR_H
