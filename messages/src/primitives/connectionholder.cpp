#include "connectionholder.h"

ConnectionHolder::ConnectionHolder(const std::string& name) :
    mConnName(name), mType(ConnectionType::UNDEF) {}

ConnectionType ConnectionHolder::getType() const {
  return mType;
}

std::string ConnectionHolder::getConnectionName() const {
  return mConnName;
}

void ConnectionHolder::setConnectionName(const std::string& name) {
  mConnName = name;
}
