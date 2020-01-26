#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include "not_owned.h"
#include "owneddeletior.h"

#include <memory>

#include <boost/dll/shared_library.hpp>

class PlaginFacade;
class PluginWidget;
class PluginConnectionSerializer;
class PluginMessageCommunicator;
class PluginConnectionInfo;

namespace plugin_support {

class PluginWidgetWrapper;
class PluginMessageCommunicatorWrapper;

class PluginInterface : public std::enable_shared_from_this<PluginInterface> {
 public:
  PluginInterface(boost::dll::shared_library&& lib, PlaginFacade* facade);

 public:
  std::unique_ptr<PluginWidgetWrapper> getWidget() noexcept;
  const PluginConnectionSerializer* getSerializer() const noexcept;
  std::unique_ptr<PluginMessageCommunicatorWrapper> getCommunicator() noexcept;
  std::shared_ptr<PluginConnectionInfo> makeConnInfo() const noexcept;

 public:
  std::string getName() const noexcept;
  std::string getId() const noexcept;

 private:
  boost::dll::shared_library mLib;
  PlaginFacade* mFacade;
};

}  // namespace plugin_support

#endif  // PLUGININTERFACE_H
