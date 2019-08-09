#ifndef CRYPTOSYSTEMDIALOGREMOVEINFORMATOR_H
#define CRYPTOSYSTEMDIALOGREMOVEINFORMATOR_H

#include <memory>

#include <persistent-storage/watchers/enqueuedevents.h>

class CryptoSystemImpl;
class Dialog;
/**
 * @brief  Отслеживает изменения в контейнере диалогов, используется для
 * актуализации данных в CryptoSystemImpl
 */
class CryptoSystemDialogRemoveInformator {
 public:
  explicit CryptoSystemDialogRemoveInformator(
      std::shared_ptr<CryptoSystemImpl> system);

 public:
  void operator()(prstorage::EnqueuedEvents event, const Dialog& dialog);

 private:
  std::shared_ptr<CryptoSystemImpl> mSystem;
};

#endif  // CRYPTOSYSTEMDIALOGREMOVEINFORMATOR_H
