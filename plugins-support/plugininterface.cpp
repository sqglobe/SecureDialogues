#include "plugininterface.h"

#include "export/plaginfacade.h"
#include "export/pluginconnectioninfo.h"
#include "export/pluginconnectionserializer.h"
#include "export/pluginmessagecommunicator.h"
#include "export/pluginwidget.h"

plugin_support::PluginInterface::PluginInterface(
    boost::dll::shared_library&& lib,
    PlaginFacade* facade) :
    mLib(std::move(lib)),
    mFacade(facade) {}

plugin_support::not_owned<PluginWidget>
plugin_support::PluginInterface::getWidget() const noexcept {
  return not_owned(this->shared_from_this(), mFacade->getWidget());
}

const PluginConnectionSerializer*
plugin_support::PluginInterface::getSerializer() const noexcept {
  return mFacade->getSerializer();
}

std::unique_ptr<PluginMessageCommunicator, plugin_support::owned_deletor>
plugin_support::PluginInterface::getCommunicator() const noexcept {
  return std::unique_ptr<PluginMessageCommunicator,
                         plugin_support::owned_deletor>(
      mFacade->makeCommunicator(),
      plugin_support::owned_deletor(shared_from_this(), mFacade));
}

std::unique_ptr<PluginConnectionInfo, plugin_support::owned_deletor>
plugin_support::PluginInterface::wrap(PluginConnectionInfo* conn) const
    noexcept {
  return std::unique_ptr<PluginConnectionInfo, plugin_support::owned_deletor>(
      conn, plugin_support::owned_deletor(shared_from_this(), mFacade));
}

std::string plugin_support::PluginInterface::getName() const noexcept {
  return mFacade->getPluginName();
}

std::string plugin_support::PluginInterface::getId() const noexcept {
  return mFacade->getId();
}
