#include "cryptosystemcontactupdateinformator.h"
#include "cryptosystemimpl.h"
#include "primitives/contact.h"

CryptoSystemContactUpdateInformator::CryptoSystemContactUpdateInformator(
    std::shared_ptr<CryptoSystemImpl> system) :
    mSystem(std::move(system)) {}

void CryptoSystemContactUpdateInformator::added(
    const ChangeListener::element& element) {
  mSystem->updateContact(element);
}

void CryptoSystemContactUpdateInformator::changed(
    const ChangeListener::element& element) {
  mSystem->updateContact(element);
}

void CryptoSystemContactUpdateInformator::removed(
    const ChangeListener::element& element) {
  mSystem->removeContact(element);
}
