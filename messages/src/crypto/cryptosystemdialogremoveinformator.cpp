#include "cryptosystemdialogremoveinformator.h"

#include "cryptosystemimpl.h"
CryptoSystemDialogRemoveInformator::CryptoSystemDialogRemoveInformator(
    std::shared_ptr<CryptoSystemImpl> system) :
    mSystem(std::move(system)) {}

void CryptoSystemDialogRemoveInformator::added(const ChangeWatcher::element&) {}

void CryptoSystemDialogRemoveInformator::changed(
    const ChangeWatcher::element&) {}

void CryptoSystemDialogRemoveInformator::removed(
    const ChangeWatcher::element& obj) {
  mSystem->dialogRemoved(obj->getDialogId());
}
