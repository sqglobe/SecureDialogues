#include "chaneladapterfactory.h"
#include <thread>

#include "adapters/oauthadapter.h"
#include "adapters/udpadapter.h"

#include "interfaces/abstractchanneladapter.h"
#include "interfaces/abstractusernotifier.h"
#include "primitives/connectionholder.h"

class FakeChannelAdapter : public AbstractChannelAdapter {
 public:
  FakeChannelAdapter(const std::string& message,
                     const std::shared_ptr<AbstractUserNotifier>& notifier) :
      AbstractChannelAdapter(notifier) {
    mMessage = message;
  }
  virtual void send(const std::string& message,
                    const std::string& adress) override {}
  virtual std::pair<std::string, std::string> receive() override {
    if (isFirst) {
      isFirst = false;
    } else {
      std::this_thread::sleep_for(std::chrono::seconds(60));
    }
    return std::make_pair("test", mMessage);
  }
  virtual bool connect(const ConnectionHolder& conn) override { return true; }

  virtual bool isConnected() const noexcept override {
    return this->AbstractChannelAdapter::isConnected();
  }

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
  AbstractChannelAdapter* adapter = nullptr;

  try {
    switch (conn.getType()) {
      case T::UDP:
        adapter = new UdpAdapter(mNotifier);
        break;
      case T::GMAIL:
      case T::VK:
        adapter = new OauthAdapter(mNotifier);
        break;
      default:
        adapter = new FakeChannelAdapter("test", mNotifier);
    }
    adapter->connect(conn);
    return std::unique_ptr<AbstractChannelAdapter>(adapter);
  } catch (...) {
    if (nullptr != adapter) {
      delete adapter;
    }
    throw;
  }
}
