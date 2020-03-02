#ifndef CONNECTSTORAGELISTENER_H
#define CONNECTSTORAGELISTENER_H

#include "communication/channel.h"
#include "interfaces/changelistener.h"
#include "primitives/connectionholder.h"
#include "storages.h"

class AbstractMessageMarshaller;
class MessageDespatcher;
class AbstractChannelAdapter;

class ConnectStorageListener : public ChangeListener<ConnectionHolder> {
 public:
  ConnectStorageListener(std::shared_ptr<MessageDespatcher> dispatcher,
                         std::function<std::unique_ptr<AbstractChannelAdapter>(
                             const ConnectionHolder&)> fabric,
                         std::shared_ptr<AbstractMessageMarshaller> marshaller,
                         std::shared_ptr<Channel::EventQueue> eventQueue,
                         const std::vector<ConnectionHolder>& elements);

 public:
  void added(const element& obj) override;
  void changed(const element& obj) override;
  void removed(const element& obj) override;

 private:
  std::shared_ptr<MessageDespatcher> mDespatcher;
  std::function<std::unique_ptr<AbstractChannelAdapter>(
      const ConnectionHolder&)>
      mFabric;
  std::shared_ptr<AbstractMessageMarshaller> mMarshaller;
  std::shared_ptr<Channel::EventQueue> mEventQueue;
};

#endif  // CONNECTSTORAGELISTENER_H
