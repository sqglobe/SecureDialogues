#include "connectionholder.h"

#include <cstring>
#include <regex>

ConnectionHolder::ConnectionHolder(std::string name) :
    mConnName(std::move(name)), mType(ConnectionType::UNDEF) {}

ConnectionType ConnectionHolder::getType() const {
  return mType;
}

std::string ConnectionHolder::getConnectionName() const {
  return mConnName;
}

void ConnectionHolder::setConnectionName(const std::string& name) {
  mConnName = name;
}

bool is_address_valid(const char* address, ConnectionType type) {
  if (std::strlen(address) == 0) {
    return false;
  }

  switch (type) {
    case ConnectionType::VK: {
      std::regex regex("^(https://vk.com/id|id)?\\d+$");
      return std::regex_match(address, regex);
    }
    case ConnectionType::UDP: {
      std::regex regex("\\d+\\.\\d+\\.\\d+\\.\\d+");
      return std::regex_match(address, regex);
    }
    case ConnectionType::GMAIL:
      [[fallthrough]];
    case ConnectionType::EMAIL: {
      std::regex regex(
          "(\\S+)@([a-z0-9-]+)(\\.)([a-z]{2,4})(\\.?)([a-z]{0,4})+");
      return std::regex_match(address, regex);
    }
    default:
      assert(false);
  }
}
