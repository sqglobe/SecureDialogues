#ifndef PLUGINWIDGETWRAPPER_H
#define PLUGINWIDGETWRAPPER_H

#include <memory>
#include <string>

#include "owneddeletior.h"

class PluginWidget;
class PluginConnectionInfo;

namespace plugin_support {

class PluginInterface;

class PluginWidgetWrapper {
 public:
  PluginWidgetWrapper(PluginWidget* widget,
                      std::shared_ptr<PluginInterface> interface);

 public:
  std::shared_ptr<PluginConnectionInfo> makeConnection() noexcept(false);
  void fillConnection(std::shared_ptr<PluginConnectionInfo> conn) noexcept(
      false);
  void setConnectionInfo(
      std::shared_ptr<PluginConnectionInfo> connInfo) noexcept;

  void cleareWidget() noexcept;
  void makeActivated() noexcept;

 private:
  PluginWidget* mWidget;
  std::shared_ptr<PluginInterface> mInterface;
};

}  // namespace plugin_support
#endif  // PLUGINWIDGETWRAPPER_H
