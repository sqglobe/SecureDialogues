#include "chaneladapterfactory.h"
#include <thread>

#include "adapters/pluginadapter.h"

#include "export/pluginconnectioninfo.h"
#include "interfaces/abstractchanneladapter.h"
#include "interfaces/abstractusernotifier.h"
#include "plugininterface.h"
#include "primitives/connectionholder.h"

class FakeChannelAdapter : public AbstractChannelAdapter {
 public:
  FakeChannelAdapter(const std::string& message,
                     const std::shared_ptr<AbstractUserNotifier>& notifier) :
      AbstractChannelAdapter(notifier, ConnectionHolder("test")) {
    mMessage = message;
  }
  void send(const std::string&, const std::string&) override {}
  std::list<std::pair<std::string, std::string>> receive() override {
    if (isFirst) {
      isFirst = false;
    } else {
      std::this_thread::sleep_for(std::chrono::seconds(60));
    }
    return {std::make_pair("test", mMessage)};
  }
  bool connect() override { return true; }

 private:
  std::string mMessage;
  bool isFirst{true};
};

ChanelAdapterFactory::ChanelAdapterFactory(
    std::shared_ptr<AbstractUserNotifier> notifier,
    std::shared_ptr<const plugin_support::PluginsContainer> container) :
    mNotifier(std::move(notifier)),
    mContainer(std::move(container)) {}

std::unique_ptr<AbstractChannelAdapter> ChanelAdapterFactory::operator()(
    const ConnectionHolder& conn) {
  auto interface = mContainer->get(conn.pluginConnInfo()->getPluginId());
  if (interface) {
    return std::make_unique<PluginAdapter>(interface->getCommunicator(),
                                           mNotifier, conn);
  }
  return {};
}
