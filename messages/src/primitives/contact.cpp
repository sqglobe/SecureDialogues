#include "contact.h"
#include "utils.h"

Contact::Contact(const std::string& channelMoniker,
                 const std::string& name,
                 const std::string& adress,
                 const std::string& key) :
    Contact(channelMoniker, name, adress, key, make_uiid()) {}

Contact::Contact(const std::string& channelMoniker,
                 const std::string& name,
                 const std::string& adress,
                 const std::string& key,
                 const std::string& id) :
    mChannelMoniker(channelMoniker),
    mName(name), mAdress(adress), mContactId(id), mPublicKey(key) {}

std::string Contact::name() const {
  return mName;
}

std::string Contact::channelMoniker() const {
  return mChannelMoniker;
}

std::string Contact::adress() const {
  return mAdress;
}

std::string Contact::id() const {
  return mContactId;
}

std::string Contact::publicKey() const {
  return mPublicKey;
}

void Contact::name(const std::string& name) {
  mName = name;
}

void Contact::channelMoniker(const std::string& moniker) {
  mChannelMoniker = moniker;
}

void Contact::adress(const std::string& adress) {
  mAdress = adress;
}

void Contact::publicKey(const std::string& key) {
  mPublicKey = key;
}
