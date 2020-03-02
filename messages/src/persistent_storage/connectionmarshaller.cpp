#include "connectionmarshaller.h"

#include <cstring>
#include "export/pluginconnectioninfo.h"
#include "export/pluginconnectionserializer.h"
#include "persistent-storage/utils/store_primitives.h"
#include "plugininterface.h"
#include "store_helpers.h"
/*
  std::string userName;
  std::string password;
  bool tlsUsed;
  std::string from;
  */

ConnectionMarshaller::ConnectionMarshaller(
    std::shared_ptr<plugin_support::PluginsContainer> container) :
    mContainer(std::move(container)) {}

void ConnectionMarshaller::restore(ConnectionHolder& elem, const void* src) {
  std::string name;
  std::string pluginId;
  src = prstorage::restore_str(name, src);
  src = prstorage::restore_str(pluginId, src);
  elem.setConnName(name);
  if (const auto plugin = mContainer->get(pluginId); plugin) {
    auto connInfo = plugin->makeConnInfo();
    plugin->getSerializer()->deserialize(src, connInfo.get());
    elem.setPluginConnInfo(std::move(connInfo));
  }
}

uint32_t ConnectionMarshaller::size(const ConnectionHolder& element) {
  uint32_t size = 0;
  const std::string pluginId = element.pluginConnInfo()->getPluginId();
  size += sizeof(std::string::size_type) + pluginId.size();
  size += sizeof(std::string::size_type) + element.connName().size();

  if (const auto plugin = mContainer->get(pluginId); plugin) {
    size += plugin->getSerializer()->get_size(element.pluginConnInfo().get());
  }

  return size;
}

void ConnectionMarshaller::store(void* dest, const ConnectionHolder& elem) {
  dest = prstorage::save_str(elem.connName(), dest);
  const std::string pluginId = elem.pluginConnInfo()->getPluginId();
  dest = prstorage::save_str(pluginId, dest);

  if (const auto plugin = mContainer->get(pluginId); plugin) {
    plugin->getSerializer()->serialize(dest, elem.pluginConnInfo().get());
  }
}
