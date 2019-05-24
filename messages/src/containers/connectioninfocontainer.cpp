#include "connectioninfocontainer.h"
#include "interfaces/changewatcher.h"
std::string get_id(const ConnectionInfoContainer::element& elem) {
  return elem.getConnectionName();
}

void ConnectionInfoContainer::updateConnectionStatus(
    Channel::ChannelStatus newStatus,
    const std::string& channelName,
    const std::string& message) {
  if (!has(channelName)) {
    return;
  }

  auto wrp = wrapper(channelName);
  wrp->setStatus(newStatus);
  wrp->setMessage(message);
  wrp.save();
}
