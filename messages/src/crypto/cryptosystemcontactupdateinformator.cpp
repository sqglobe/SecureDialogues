#include "cryptosystemcontactupdateinformator.h"
#include "cryptosystemimpl.h"

CryptoSystemContactUpdateInformator::CryptoSystemContactUpdateInformator(
    std::shared_ptr<CryptoSystemImpl> system) :
    mSystem(std::move(system)) {}

void CryptoSystemContactUpdateInformator::added(
    const ChangeWatcher::element& obj) {
  mSystem->updateContact(obj);
}

void CryptoSystemContactUpdateInformator::changed(
    const ChangeWatcher::element& obj) {
  mSystem->updateContact(obj);
}

void CryptoSystemContactUpdateInformator::removed(
    const ChangeWatcher::element& obj) {
  mSystem->removeContact(obj);
}
