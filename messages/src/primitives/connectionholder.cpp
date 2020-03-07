#include "connectionholder.h"

#include <cstring>
#include "export/pluginconnectioninfo.h"

ConnectionHolder::ConnectionHolder(
    const std::string& connName,
    std::shared_ptr<PluginConnectionInfo> pluginConnInfo) :
    mConnName(connName),
    mPluginConnInfo(std::move(pluginConnInfo)) {}

ConnectionHolder::ConnectionHolder(const std::string& connName) :
    mConnName(connName) {}

std::string ConnectionHolder::connName() const {
  return mConnName;
}

void ConnectionHolder::setConnName(const std::string& connName) {
  mConnName = connName;
}

std::shared_ptr<PluginConnectionInfo> ConnectionHolder::pluginConnInfo() const {
  return mPluginConnInfo;
}

void ConnectionHolder::setPluginConnInfo(
    const std::shared_ptr<PluginConnectionInfo>& pluginConnInfo) {
  mPluginConnInfo = pluginConnInfo;
}
