#ifndef DIALOGCREATIONANDMESSAGESEXCHANGECORESTAND_H
#define DIALOGCREATIONANDMESSAGESEXCHANGECORESTAND_H

#include "interfaces/changelistener.h"
#include "interfaces/messageconteinereventhandler.h"

#include <memory>

#include "communication/dialogactionhandler.h"
#include "communication/messageactionhandler.h"
#include "communication/messagedespatcher.h"
#include "containers/messagecontainer.h"
#include "containers/storages.h"

inline Contact make_contact(std::string channelMoniker,
                            std::string name,
                            std::string adress,
                            std::string key,
                            std::string id) {
  return Contact(std::move(channelMoniker), std::move(name), std::move(adress),
                 std::move(key), std::move(id));
}

class DialogCreationAndMessagesExchangeCoreStand {
 public:
  DialogCreationAndMessagesExchangeCoreStand(std::string folder);
  ~DialogCreationAndMessagesExchangeCoreStand();

 public:
  void addChannelAdapter(AbstractChannelAdapter* adapter, std::string name);

 public:
  std::shared_ptr<MessageActionHandler> messageActionHandler() const;

  std::shared_ptr<DialogActionHandler> dialogActionHandler() const;

  std::shared_ptr<DialogStorage> dialogStorage() const;

  std::shared_ptr<MessageContainer> messageContainer() const;

 private:
  inline void fillContainers();

 private:
  std::shared_ptr<ConnectionStorage> mConnectionStorage;
  std::shared_ptr<ContactStorage> mContactStorage;
  std::shared_ptr<DialogStorage> mDialogStorage;
  std::shared_ptr<MessageContainer> mMessageContainer;
  std::shared_ptr<MessageDespatcher> mMessageDispatcher;
  std::shared_ptr<MessageActionHandler> mMessageActionHandler;
  std::shared_ptr<DialogActionHandler> mDialogActionHandler;
  std::string mFolder;
};

void DialogCreationAndMessagesExchangeCoreStand::fillContainers() {
  mConnectionStorage->add(ConnectionHolder("Test 1"));
  mConnectionStorage->add(ConnectionHolder("Test 2"));
  mConnectionStorage->add(ConnectionHolder("Test 3"));

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

#endif  // DIALOGCREATIONANDMESSAGESEXCHANGECORESTAND_H
