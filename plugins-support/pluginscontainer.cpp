#include "pluginscontainer.h"
#include "plugininterface.h"

void plugin_support::PluginsContainer::add(
    std::shared_ptr<PluginInterface> plugin) {
  mPlugins.emplace(plugin->getId(), std::move(plugin));
}

std::shared_ptr<plugin_support::PluginInterface>
plugin_support::PluginsContainer::get(const std::string& id) const {
  if (auto it = mPlugins.find(id); it != mPlugins.end()) {
    return it->second;
  }

  return {};
}

plugin_support::PluginsContainer::const_iterator
plugin_support::PluginsContainer::cbegin() const {
  return const_iterator(mPlugins.cbegin());
}

plugin_support::PluginsContainer::const_iterator
plugin_support::PluginsContainer::cend() const {
  return const_iterator(mPlugins.cend());
}
