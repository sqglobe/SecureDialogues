#ifndef FAKECHANNELADAPTERSENDMESSAGEONLY_H
#define FAKECHANNELADAPTERSENDMESSAGEONLY_H
#include <thread>
#include "fakenotifier.h"
#include "interfaces/abstractchanneladapter.h"

class FakeChannelAdapterSendMessageOnly : public AbstractChannelAdapter {
 public:
  FakeChannelAdapterSendMessageOnly() :
      AbstractChannelAdapter(std::shared_ptr<FakeNotifier>(new FakeNotifier),
                             ConnectionHolder("test")) {}
  virtual void send(const std::string& message,
                    const std::string& adress) override {
    mMessage = message;
    mAdress = adress;
  }
  virtual std::pair<std::string, std::string> receive() override {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    return std::make_pair("test", "fake");
  }
  virtual bool connect() override { return true; }

 public:
  std::string mMessage;
  std::string mAdress;
};
#endif  // FAKECHANNELADAPTERSENDMESSAGEONLY_H
