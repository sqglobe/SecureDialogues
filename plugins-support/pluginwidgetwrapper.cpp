#include "pluginwidgetwrapper.h"
#include "export/pluginwidget.h"
#include "plugininterface.h"

PluginWidgetWrapper::PluginWidgetWrapper(
    PluginWidget* widget,
    std::shared_ptr<PluginInterface> interface) :
    mWidget(widget),
    mInterface(std::move(interface)) {}

std::string PluginWidgetWrapper::getErrorMessage() const noexcept {
  return mWidget->getErrorMessage();
}

std::shared_ptr<PluginConnectionInfo>
PluginWidgetWrapper::makeConnectionInfo() noexcept {
  return std::make_shared<PluginConnectionInfo>(mWidget->)
}
