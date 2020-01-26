#ifndef PLUGINWIDGETWRAPPER_H
#define PLUGINWIDGETWRAPPER_H

#include <memory>
#include <string>

#include "owneddeletior.h"

class PluginWidget;
class PluginInterface;
class PluginConnectionInfo;

class PluginWidgetWrapper {
 public:
  PluginWidgetWrapper(PluginWidget* widget,
                      std::shared_ptr<PluginInterface> interface);

 public:
  std::string getErrorMessage() const noexcept;
  std::shared_ptr<PluginConnectionInfo> makeConnectionInfo() noexcept;
  void setConnectionInfo(
      std::shared_ptr<PluginConnectionInfo> connInfo) noexcept;

  void cleareWidget() noexcept;
  void makeActivated() noexcept;

 private:
  PluginWidget* mWidget;
  std::shared_ptr<PluginInterface> mInterface;
  std::shared_ptr<PluginConnectionInfo> mConn;
};

#endif  // PLUGINWIDGETWRAPPER_H
