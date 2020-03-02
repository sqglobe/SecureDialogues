#ifndef CONNECTIONMARSHALLER_H
#define CONNECTIONMARSHALLER_H

#include "pluginscontainer.h"
#include "primitives/connectionholder.h"

class ConnectionMarshaller {
 public:
  ConnectionMarshaller(
      std::shared_ptr<plugin_support::PluginsContainer> container);

 public:
  void restore(ConnectionHolder& elem, const void* src);
  uint32_t size(const ConnectionHolder& element);
  void store(void* dest, const ConnectionHolder& elem);

 private:
  std::shared_ptr<plugin_support::PluginsContainer> mContainer;
};

#endif  // CONNECTIONMARSHALLER_H
