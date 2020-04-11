#include "pluginwidgetwrapper.h"
#include "export/pluginwidget.h"
#include "plugininterface.h"
#include "support-functions.h"
plugin_support::PluginWidgetWrapper::PluginWidgetWrapper(
    PluginWidget* widget,
    std::shared_ptr<PluginInterface> pInterface) :
    mWidget(widget),
    mInterface(std::move(pInterface)) {}

std::shared_ptr<PluginConnectionInfo>
plugin_support::PluginWidgetWrapper::makeConnection() noexcept(false) {
  auto conn = mInterface->makeConnInfo();
  if (auto err = make_string(mWidget->fillConnectionInfo(conn.get()));
      !err.empty()) {
    throw std::runtime_error(err);
  }
  return conn;
}

void plugin_support::PluginWidgetWrapper::fillConnection(
    std::shared_ptr<PluginConnectionInfo> conn) noexcept(false) {
  if (auto err = make_string(mWidget->fillConnectionInfo(conn.get()));
      !err.empty()) {
    throw std::runtime_error(err);
  }
}

void plugin_support::PluginWidgetWrapper::setConnectionInfo(
    std::shared_ptr<PluginConnectionInfo> connInfo) noexcept {
  mWidget->setConnectionInfo(connInfo.get());
}

void plugin_support::PluginWidgetWrapper::cleareWidget() noexcept {
  mWidget->cleareWidget();
}

void plugin_support::PluginWidgetWrapper::makeActivated() noexcept {
  mWidget->makeActivated();
}

PluginWidget* plugin_support::PluginWidgetWrapper::getWidget() const noexcept {
  return mWidget;
}

std::string plugin_support::PluginWidgetWrapper::getId() const noexcept {
  return mInterface->getId();
}
