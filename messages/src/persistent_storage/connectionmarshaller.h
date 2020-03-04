#ifndef CONNECTIONMARSHALLER_H
#define CONNECTIONMARSHALLER_H

#include "pluginscontainer.h"
#include "primitives/connectionholder.h"

class ConnectionMarshaller {
 public:
  static void init(
      std::shared_ptr<const plugin_support::PluginsContainer> container);

 public:
  static void restore(ConnectionHolder& elem, const void* src);
  static uint32_t size(const ConnectionHolder& element);
  static void store(void* dest, const ConnectionHolder& elem);

 private:
  static std::shared_ptr<const plugin_support::PluginsContainer> mContainer;
};

#endif  // CONNECTIONMARSHALLER_H
