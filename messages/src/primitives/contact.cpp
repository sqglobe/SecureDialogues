#include "contact.h"

std::string_view Contact::name() const noexcept {
  return mName;
}

std::string_view Contact::channelMoniker() const noexcept {
  return mChannelMoniker;
}

std::string_view Contact::adress() const noexcept {
  return mAdress;
}

std::string_view Contact::id() const noexcept {
  return mContactId;
}

std::string_view Contact::publicKey() const noexcept {
  return mPublicKey;
}

void Contact::name(std::string name) {
  mName = std::move(name);
}

void Contact::channelMoniker(std::string moniker) {
  mChannelMoniker = std::move(moniker);
}

void Contact::adress(std::string adress) {
  mAdress = std::move(adress);
}

void Contact::publicKey(std::string key) {
  mPublicKey = std::move(key);
}
