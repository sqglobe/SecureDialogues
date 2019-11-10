#ifndef SYSTEMMESSAGEGENERATOR_H
#define SYSTEMMESSAGEGENERATOR_H

#include <memory>
#include "interfaces/changelistener.h"

class Dialog;
class MessageContainer;

class SystemMessageGenerator : public ChangeListener<Dialog> {
 public:
  explicit SystemMessageGenerator(std::weak_ptr<MessageContainer> container);

 public:
  void added(const element&) override;
  void changed(const element& obj) override;
  void removed(const element&) override;

 private:
  std::weak_ptr<MessageContainer> mMessageContainer;
};

#endif  // SYSTEMMESSAGEGENERATOR_H
