#include "recievedcontactsstoragewrapper.h"

RecievedContactsStorageWrapper::RecievedContactsStorageWrapper(
    std::shared_ptr<DiscoveredContactStorage> discoveredContactStorage,
    std::shared_ptr<ContactStorage> contactStorage) :
    mDiscoveredContactStorage(std::move(discoveredContactStorage)),
    mContactStorage(std::move(contactStorage)) {}

DiscoveredContact RecievedContactsStorageWrapper::getDiscoveredContact(
    const std::string& dialog_id) const {
  return mDiscoveredContactStorage->get(dialog_id);
}

Contact RecievedContactsStorageWrapper::getContact(
    const std::string& dialog_id) const {
  const auto discovered = mDiscoveredContactStorage->get(dialog_id);
  return Contact(std::string(discovered.channel_moniker()),
                 std::string(discovered.name()),
                 std::string(discovered.contact_adress()),
                 std::string(discovered.public_key()));
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
