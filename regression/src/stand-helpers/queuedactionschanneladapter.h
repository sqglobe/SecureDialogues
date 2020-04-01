#ifndef QUEUEDACTIONSCHANNELADAPTER_H
#define QUEUEDACTIONSCHANNELADAPTER_H

#include <memory>
#include <optional>
#include "interfaces/abstractchanneladapter.h"
class QueuedActionsChannelAdapter : public AbstractChannelAdapter {
  class Sender;
  class Reciever;

 public:
  struct MessageData {
    std::string address;
    std::string message;
  };

 public:
  QueuedActionsChannelAdapter(std::shared_ptr<AbstractUserNotifier> notifier,
                              ConnectionHolder conn);

 public:
  void send(const std::string& message,
            const std::string& adress) noexcept(false) override;
  std::list<std::pair<std::string, std::string>> receive() noexcept(
      false) override;
  bool connect() override;

 public:
  std::optional<MessageData> recieveMessageFromApplication(int milliseconds);
  void sendMessageToApplication(const MessageData& data);

 private:
  std::shared_ptr<Sender> mSender;
  std::shared_ptr<Reciever> mReciever;
};

#endif  // QUEUEDACTIONSCHANNELADAPTER_H
