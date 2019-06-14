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
    const std::shared_ptr<MessageContainer>& container) :
    mMessageContainer(container) {}

void SystemMessageGenerator::changed(const ChangeWatcher::element& obj) {
  if (isInSystemMessageState(obj->getStatus())) {
    auto text = get_message(obj->getStatus());
    if (auto lock = mMessageContainer.lock()) {
      lock->addMessage(
          obj->getDialogId(),
          std::make_shared<UserMessage>(UserMessage::Status::DELIVERIED,
                                        UserMessage::Type::SYSTEM, text));
    }
  }
}
