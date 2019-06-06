#include "chaneladapterfactory.h"
#include <thread>

#include "adapters/emailadapter.h"
#include "adapters/oauthadapter.h"
#include "adapters/udpadapter.h"

#include "interfaces/abstractchanneladapter.h"
#include "interfaces/abstractusernotifier.h"
#include "primitives/connectionholder.h"

class FakeChannelAdapter : public AbstractChannelAdapter {
 public:
  FakeChannelAdapter(const std::string& message,
                     const std::shared_ptr<AbstractUserNotifier>& notifier) :
      AbstractChannelAdapter(notifier, ConnectionHolder("test")) {
    mMessage = message;
  }
  void send(const std::string&, const std::string&) override {}
  virtual std::pair<std::string, std::string> receive() override {
    if (isFirst) {
      isFirst = false;
    } else {
      std::this_thread::sleep_for(std::chrono::seconds(60));
    }
    return std::make_pair("test", mMessage);
  }
  bool connect() override { return true; }

 private:
  std::string mMessage;
  bool isFirst{true};
};

ChanelAdapterFactory::ChanelAdapterFactory(
    const std::shared_ptr<AbstractUserNotifier>& notifier) :
    mNotifier(notifier) {}

std::unique_ptr<AbstractChannelAdapter> ChanelAdapterFactory::operator()(
    const ConnectionHolder& conn) {
  using T = ConnectionType;
  switch (conn.getType()) {
    case T::UDP:
      return std::make_unique<UdpAdapter>(mNotifier, conn);
    case T::GMAIL:
    case T::VK:
      return std::make_unique<OauthAdapter>(mNotifier, conn);
    case T::EMAIL:
      return std::make_unique<EmailAdapter>(mNotifier, conn);
    default:
      return std::make_unique<FakeChannelAdapter>("test", mNotifier);
  }
}
