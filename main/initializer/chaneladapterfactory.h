#ifndef CHANELADAPTERFACTORY_H
#define CHANELADAPTERFACTORY_H

#include <memory>

class AbstractUserNotifier;
class AbstractChannelAdapter;
class ConnectionHolder;

class ChanelAdapterFactory {
 public:
  ChanelAdapterFactory(const std::shared_ptr<AbstractUserNotifier>& notifier);

 public:
  std::unique_ptr<AbstractChannelAdapter> operator()(
      const ConnectionHolder& conn);

 private:
  std::shared_ptr<AbstractUserNotifier> mNotifier;
};

#endif  // CHANELADAPTERFACTORY_H
