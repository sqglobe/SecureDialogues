#ifndef PLUGINDISCOVERY_H
#define PLUGINDISCOVERY_H

#include <string>

namespace plugin_support {
class PluginsContainer;
void discover_plugins(const std::string& path, PluginsContainer& container);
}  // namespace plugin_support

#endif  // PLUGINDISCOVERY_H
