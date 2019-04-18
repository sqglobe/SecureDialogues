#include "cryptosystemcontactupdateinformator.h"
#include "cryptosystemimpl.h"

CryptoSystemContactUpdateInformator::CryptoSystemContactUpdateInformator(
    const std::shared_ptr<CryptoSystemImpl>& system) :
    mSystem(system) {}

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
