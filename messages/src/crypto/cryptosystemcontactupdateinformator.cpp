#include "cryptosystemcontactupdateinformator.h"
#include "cryptosystemimpl.h"
#include "primitives/contact.h"

CryptoSystemContactUpdateInformator::CryptoSystemContactUpdateInformator(
    std::shared_ptr<CryptoSystemImpl> system) :
    mSystem(std::move(system)) {}

void CryptoSystemContactUpdateInformator::operator()(
    prstorage::EnqueuedEvents event,
    const Contact& contact) {
  if (event == prstorage::EnqueuedEvents::ADDED ||
      event == prstorage::EnqueuedEvents::UPDATED) {
    mSystem->updateContact(contact);
  } else {
    mSystem->removeContact(contact);
  }
}
