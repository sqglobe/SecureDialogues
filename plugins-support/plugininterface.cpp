#include "plugininterface.h"

#include "export/pluginconnectioninfo.h"
#include "export/pluginconnectionserializer.h"
#include "export/pluginfacade.h"
#include "export/pluginmessagecommunicator.h"
#include "export/pluginwidget.h"
#include "pluginmessagecommunicatorwrapper.h"
#include "pluginwidgetwrapper.h"

plugin_support::PluginInterface::PluginInterface(
    boost::dll::shared_library&& lib,
    PluginFacade* facade) :
    mLib(std::move(lib)),
    mFacade(facade) {}

std::unique_ptr<plugin_support::PluginWidgetWrapper>
plugin_support::PluginInterface::getWidget() noexcept {
  return std::make_unique<plugin_support::PluginWidgetWrapper>(
      mFacade->getWidget(), shared_from_this());
}

const PluginConnectionSerializer*
plugin_support::PluginInterface::getSerializer() const noexcept {
  return mFacade->getSerializer();
}

std::shared_ptr<const PluginAddressValidator>
plugin_support::PluginInterface::getAddressValidator() const noexcept {
  return std::shared_ptr<const PluginAddressValidator>(
      mFacade->getAddressValidator(),
      [interface = this->shared_from_this()](const auto*) {

      });
}

std::unique_ptr<plugin_support::PluginMessageCommunicatorWrapper>
plugin_support::PluginInterface::getCommunicator() noexcept {
  auto comm = std::unique_ptr<PluginMessageCommunicator, owned_deletor>(
      mFacade->makeCommunicator(), owned_deletor(mFacade));
  return std::make_unique<plugin_support::PluginMessageCommunicatorWrapper>(
      std::move(comm), shared_from_this(), mFacade);
}

std::shared_ptr<PluginConnectionInfo>
plugin_support::PluginInterface::makeConnInfo() const noexcept {
  return std::shared_ptr<PluginConnectionInfo>(
      mFacade->makeEmptyConn(), plugin_support::owned_deletor(mFacade));
}

std::string plugin_support::PluginInterface::getName() const noexcept {
  return mFacade->getPluginName();
}

std::string plugin_support::PluginInterface::getId() const noexcept {
  return mFacade->getId();
}

std::string plugin_support::PluginInterface::getTranslationFileName(
    Language lang) const noexcept {
  return mFacade->getTranslationFileName(lang);
}

std::string plugin_support::PluginInterface::getGettextDomain() const noexcept {
  return mFacade->getGettextDomain();
}
