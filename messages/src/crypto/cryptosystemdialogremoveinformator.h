#ifndef CRYPTOSYSTEMDIALOGREMOVEINFORMATOR_H
#define CRYPTOSYSTEMDIALOGREMOVEINFORMATOR_H

#include <memory>

#include "interfaces/changelistener.h"
#include "primitives/dialog.h"

class CryptoSystemImpl;
class Dialog;
/**
 * @brief  Отслеживает изменения в контейнере диалогов, используется для
 * актуализации данных в CryptoSystemImpl
 */
class CryptoSystemDialogRemoveInformator : public ChangeListener<Dialog> {
 public:
  explicit CryptoSystemDialogRemoveInformator(
      std::shared_ptr<CryptoSystemImpl> system);

 public:
  void added(const element&) override {}
  void changed(const element&) override {}
  void removed(const element& obj) override;

 private:
  std::shared_ptr<CryptoSystemImpl> mSystem;
};

#endif  // CRYPTOSYSTEMDIALOGREMOVEINFORMATOR_H
