#include "cryptosystemdialogremoveinformator.h"

#include "cryptosystemimpl.h"
CryptoSystemDialogRemoveInformator::CryptoSystemDialogRemoveInformator(
    const std::shared_ptr<CryptoSystemImpl>& system) :
    mSystem(system) {}

void CryptoSystemDialogRemoveInformator::added(
    const ChangeWatcher::element& obj) {
  (void)obj;
}

void CryptoSystemDialogRemoveInformator::changed(
    const ChangeWatcher::element& obj) {
  (void)obj;
}

void CryptoSystemDialogRemoveInformator::removed(
    const ChangeWatcher::element& obj) {
  mSystem->dialogRemoved(obj->getDialogId());
}
