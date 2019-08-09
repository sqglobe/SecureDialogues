#ifndef SYSTEMMESSAGEGENERATOR_H
#define SYSTEMMESSAGEGENERATOR_H

#include <persistent-storage/watchers/enqueuedevents.h>
#include <memory>

class Dialog;
class MessageContainer;

class SystemMessageGenerator {
 public:
  explicit SystemMessageGenerator(std::weak_ptr<MessageContainer> container);

 public:
  void operator()(prstorage::EnqueuedEvents event, const Dialog& dialog);

 private:
  std::weak_ptr<MessageContainer> mMessageContainer;
};

#endif  // SYSTEMMESSAGEGENERATOR_H
