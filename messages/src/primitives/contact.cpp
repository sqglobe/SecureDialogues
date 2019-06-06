#include "contact.h"
#include "utils.h"

Contact::Contact(std::string channelMoniker,
                 std::string name,
                 std::string adress,
                 std::string key) :
    mChannelMoniker(std::move(channelMoniker)),
    mName(std::move(name)), mAdress(std::move(adress)), mContactId(make_uiid()),
    mPublicKey(std::move(key)) {}

Contact::Contact(std::string channelMoniker,
                 std::string name,
                 std::string adress,
                 std::string key,
                 std::string id) :
    mChannelMoniker(std::move(channelMoniker)),
    mName(std::move(name)), mAdress(std::move(adress)),
    mContactId(std::move(id)), mPublicKey(std::move(key)) {}

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
