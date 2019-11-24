#include "connectionholder.h"

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
