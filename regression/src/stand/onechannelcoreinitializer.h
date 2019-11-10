#ifndef ONECHANNELCOREINITIALIZER_H
#define ONECHANNELCOREINITIALIZER_H

#include "interfaces/abstractcoreinitializer.h"

inline Contact make_contact(std::string channelMoniker,
                            std::string name,
                            std::string adress,
                            std::string key,
                            std::string id) {
  return Contact(std::move(channelMoniker), std::move(name), std::move(adress),
                 std::move(key), std::move(id));
}

class MessageDespatcher;
class QueuedActionsChannelAdapter;

class OneChannelCoreInitializer : public AbstractCoreInitializer {
 public:
  OneChannelCoreInitializer(const std::string& folder);

 public:
  void init(const std::shared_ptr<AbstractUserNotifier>& notifier) override;
  void saveFiles() override;
  void startMessagesHandling(
      const std::shared_ptr<AbstractUserNotifier>&,
      const std::shared_ptr<Channel::EventQueue>&) override;

 public:
  std::shared_ptr<ConnectionStorage> getConnectionStorage() const override;
  std::shared_ptr<ContactStorage> getContactStorage() const override;
  std::shared_ptr<CryptoSystemImpl> getCryptoSystem() const override;
  std::shared_ptr<DialogActionHandler> getDialogActionHandler() const override;
  std::shared_ptr<DialogStorage> getDialogStorage() const override;
  std::shared_ptr<MessageActionHandler> getMessageActionHandler()
      const override;
  std::shared_ptr<MessageContainer> getMessageContainer() const override;

 public:
  std::pair<QueuedActionsChannelAdapter*, int> make_dialog(
      const std::string& addres,
      const std::string& dialogId,
      const std::string& channel);

 private:
  inline void fill();

 private:
  std::shared_ptr<ConnectionStorage> mConnectionStorage;
  std::shared_ptr<ContactStorage> mContactStorage;
  std::shared_ptr<DialogStorage> mDialogStorage;
  std::shared_ptr<MessageContainer> mMessageContainer;
  std::shared_ptr<MessageDespatcher> mMessageDispatcher;
  std::shared_ptr<MessageActionHandler> mMessageActionHandler;
  std::shared_ptr<DialogActionHandler> mDialogActionHandler;
  std::shared_ptr<CryptoSystemImpl> mCryptoSystemImpl;
};

void OneChannelCoreInitializer::fill() {
  mConnectionStorage->add(ConnectionHolder(UdpConnection{}, "Test 1"));
  mConnectionStorage->add(ConnectionHolder(UdpConnection{}, "Test 2"));
  mConnectionStorage->add(ConnectionHolder(UdpConnection{}, "Test 3"));

  mContactStorage->add(
      make_contact("Test 1", "name 1", "address 1", "no key", "contact 1"));
  mContactStorage->add(
      make_contact("Test 1", "name 2", "address 2", "no key", "contact 2"));
  mContactStorage->add(
      make_contact("Test 2", "name 3", "address 3", "no key", "contact 3"));
  mContactStorage->add(
      make_contact("Test 2", "name 4", "address 4", "no key", "contact 4"));
  mContactStorage->add(
      make_contact("Test 3", "name 5", "address 5", "no key", "contact 5"));
}

#endif  // ONECHANNELCOREINITIALIZER_H
