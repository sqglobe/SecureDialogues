#ifndef CRYPTOSYSTEMDIALOGREMOVEINFORMATOR_H
#define CRYPTOSYSTEMDIALOGREMOVEINFORMATOR_H

#include "containers/dialogmanager.h"
#include "interfaces/changewatcher.h"

class CryptoSystemImpl;

/**
 * @brief  Отслеживает изменения в контейнере диалогов, используется для
 * актуализации данных в CryptoSystemImpl
 */
class CryptoSystemDialogRemoveInformator
    : public ChangeWatcher<DialogManager::const_element> {
 public:
  CryptoSystemDialogRemoveInformator(
      const std::shared_ptr<CryptoSystemImpl>& system);

 public:
  virtual void added(const element& obj) override;
  virtual void changed(const element& obj) override;
  virtual void removed(const element& obj) override;

 private:
  std::shared_ptr<CryptoSystemImpl> mSystem;
};

#endif  // CRYPTOSYSTEMDIALOGREMOVEINFORMATOR_H
