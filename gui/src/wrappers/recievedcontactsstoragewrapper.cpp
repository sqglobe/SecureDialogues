#include "recievedcontactsstoragewrapper.h"

RecievedContactsStorageWrapper::RecievedContactsStorageWrapper(
    std::shared_ptr<DiscoveredContactStorage> discoveredContactStorage,
    std::shared_ptr<ContactStorage> contactStorage) :
    mDiscoveredContactStorage(std::move(discoveredContactStorage)),
    mContactStorage(std::move(contactStorage)) {}

DiscoveredContact RecievedContactsStorageWrapper::get(
    const std::string& dialog_id) const {
  return mDiscoveredContactStorage->get(dialog_id);
}

bool RecievedContactsStorageWrapper::removeDiscoveredContact(
    const std::string& dialog_id) {
  return mDiscoveredContactStorage->remove(dialog_id);
}

bool RecievedContactsStorageWrapper::saveToContact(
    const Contact& contact,
    const std::string& dialog_id) {
  if (!mContactStorage->add(contact)) {
    return false;
  }

  if (!mDiscoveredContactStorage->remove(dialog_id)) {
    mContactStorage->remove(std::string(contact.id()));
    return false;
  }

  return true;
}
