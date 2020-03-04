#ifndef CHANELADAPTERFACTORY_H
#define CHANELADAPTERFACTORY_H

#include <pluginscontainer.h>
#include <memory>

class AbstractUserNotifier;
class AbstractChannelAdapter;
class ConnectionHolder;

class ChanelAdapterFactory {
 public:
  ChanelAdapterFactory(
      std::shared_ptr<AbstractUserNotifier> notifier,
      std::shared_ptr<const plugin_support::PluginsContainer> container);

 public:
  std::unique_ptr<AbstractChannelAdapter> operator()(
      const ConnectionHolder& conn);

 private:
  std::shared_ptr<AbstractUserNotifier> mNotifier;
  std::shared_ptr<const plugin_support::PluginsContainer> mContainer;
};

#endif  // CHANELADAPTERFACTORY_H
