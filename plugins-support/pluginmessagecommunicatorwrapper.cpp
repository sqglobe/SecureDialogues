#include "pluginmessagecommunicatorwrapper.h"
#include "export/pluginconnectioninfo.h"
#include "export/pluginmessagecommunicator.h"
#include "export/recievedmessagesiterator.h"

#include "owneddeletior.h"
#include "plugininterface.h"

plugin_support::PluginMessageCommunicatorWrapper::
    PluginMessageCommunicatorWrapper(
        std::unique_ptr<PluginMessageCommunicator, owned_deletor>&&
            communicator,
        std::shared_ptr<plugin_support::PluginInterface> pInterface,
        PluginFacade* facade) :
    mCommunicator(std::move(communicator)),
    mInterface(std::move(pInterface)), mFacade(facade) {}

PluginApiErrorCodes plugin_support::PluginMessageCommunicatorWrapper::send(
    const std::string& addressTo,
    const std::string& message) noexcept {
  return mCommunicator->send(addressTo.c_str(), message.c_str());
}

std::unique_ptr<RecievedMessagesIterator, plugin_support::owned_deletor>
plugin_support::PluginMessageCommunicatorWrapper::recieve() noexcept {
  return std::unique_ptr<RecievedMessagesIterator,
                         plugin_support::owned_deletor>(
      mCommunicator->recieve(), owned_deletor(mFacade));
}

PluginApiErrorCodes plugin_support::PluginMessageCommunicatorWrapper::connect(
    const std::shared_ptr<const PluginConnectionInfo>& connInfo) noexcept {
  return mCommunicator->connect(connInfo.get());
}
