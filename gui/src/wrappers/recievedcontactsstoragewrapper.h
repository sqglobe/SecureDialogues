#ifndef RECIEVEDCONTACTSSTORAGEWRAPPER_H
#define RECIEVEDCONTACTSSTORAGEWRAPPER_H

#include <memory>
#include "containers/storages.h"
class RecievedContactsStorageWrapper {
 public:
  RecievedContactsStorageWrapper(
      std::shared_ptr<DiscoveredContactStorage> discoveredContactStorage,
      std::shared_ptr<ContactStorage> contactStorage);

 public:
  DiscoveredContact get(const std::string& dialog_id) const;

 public:
  bool removeDiscoveredContact(const std::string& dialog_id);
  bool saveToContact(const Contact& contact, const std::string& dialog_id);

 private:
  std::shared_ptr<DiscoveredContactStorage> mDiscoveredContactStorage;
  std::shared_ptr<ContactStorage> mContactStorage;
};

#endif  // RECIEVEDCONTACTSSTORAGEWRAPPER_H
