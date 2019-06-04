#ifndef FAKECHANNELADAPTER_H
#define FAKECHANNELADAPTER_H
#include "fakenotifier.h"
#include "interfaces/abstractchanneladapter.h"

#include <thread>

class FakeChannelAdapter : public AbstractChannelAdapter {
 public:
  FakeChannelAdapter(const std::string& message) :
      AbstractChannelAdapter(std::shared_ptr<FakeNotifier>(new FakeNotifier),
                             ConnectionHolder("test")) {
    mMessage = message;
  }
  virtual void send(const std::string&, const std::string&) override {}
  virtual std::pair<std::string, std::string> receive() override {
    if (isFirst) {
      isFirst = false;
    } else {
      std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return std::make_pair("test", mMessage);
  }
  virtual bool connect() override { return true; }

 private:
  std::string mMessage;
  bool isFirst{true};
};

#endif  // FAKECHANNELADAPTER_H
