#include "systemmessagegenerator.h"
#include <cassert>
#include "containers/messagecontainer.h"
#include "containers/usermessage.h"
#include "primitives/dialog.h"

// enum class Status{NEW = 0, CREATE_REQUEST, WAIT_CONFIRM, ACCEPTED_WAIT,
// CANCEL_WAIT, ACTIVE, CLOSED, CANCELED, ABORTED};

std::string get_message(Dialog::Status status) {
  using S = Dialog::Status;
  switch (status) {
    case S::ABORTED:
      return "Диалог сброшен";
    case S::CLOSED:
      return "Диалог был закрыт";
    default:
      assert(false);
  }
}

bool isInSystemMessageState(Dialog::Status status) {
  using S = Dialog::Status;
  return S::ABORTED == status || S::CLOSED == status;
}

SystemMessageGenerator::SystemMessageGenerator(
    std::weak_ptr<MessageContainer> container) :
    mMessageContainer(std::move(container)) {}

void SystemMessageGenerator::operator()(prstorage::EnqueuedEvents event,
                                        const Dialog& dialog) {
  if (event == prstorage::EnqueuedEvents::UPDATED) {
    if (isInSystemMessageState(dialog.getStatus())) {
      auto text = get_message(dialog.getStatus());
      if (auto lock = mMessageContainer.lock()) {
        lock->addMessage(
            std::string(dialog.getDialogId()),
            std::make_shared<UserMessage>(UserMessage::Status::DELIVERIED,
                                          UserMessage::Type::SYSTEM, text));
      }
    }
  }
}
