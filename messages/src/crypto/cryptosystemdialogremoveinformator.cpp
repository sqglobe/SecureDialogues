#include "cryptosystemdialogremoveinformator.h"
#include "primitives/dialog.h"

#include "cryptosystemimpl.h"
CryptoSystemDialogRemoveInformator::CryptoSystemDialogRemoveInformator(
    std::shared_ptr<CryptoSystemImpl> system) :
    mSystem(std::move(system)) {}

void CryptoSystemDialogRemoveInformator::operator()(
    prstorage::EnqueuedEvents event,
    const Dialog& dialog) {
  if (event == prstorage::EnqueuedEvents::DELETED) {
    mSystem->dialogRemoved(dialog.getDialogId());
  }
}
