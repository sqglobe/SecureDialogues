#include "cryptosystemdialogremoveinformator.h"
#include "cryptosystemimpl.h"

CryptoSystemDialogRemoveInformator::CryptoSystemDialogRemoveInformator(
    std::shared_ptr<CryptoSystemImpl> system) :
    mSystem(std::move(system)) {}

void CryptoSystemDialogRemoveInformator::removed(
    const ChangeListener::element& obj) {
  mSystem->dialogRemoved(obj.getDialogId());
}
