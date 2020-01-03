#include "systemmessagegenerator.h"
#include <cassert>
#include "containers/messagecontainer.h"
#include "containers/usermessage.h"
#include "primitives/dialog.h"

#include <libintl.h>

// enum class Status{NEW = 0, CREATE_REQUEST, WAIT_CONFIRM, ACCEPTED_WAIT,
// CANCEL_WAIT, ACTIVE, CLOSED, CANCELED, ABORTED};

std::string get_message(Dialog::Status status) {
  using S = Dialog::Status;
  switch (status) {
    case S::ABORTED:
      return dgettext("messages", "Dialog was aborted");
    case S::CLOSED:
      return dgettext("messages", "Dialog was closed");
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

void SystemMessageGenerator::added(const ChangeListener::element&) {}

void SystemMessageGenerator::changed(const ChangeListener::element& obj) {
  if (isInSystemMessageState(obj.getStatus())) {
    auto text = get_message(obj.getStatus());
    if (auto lock = mMessageContainer.lock()) {
      lock->addMessage(
          std::string(obj.getDialogId()),
          std::make_shared<UserMessage>(UserMessage::Status::DELIVERIED,
                                        UserMessage::Type::SYSTEM, text));
    }
  }
}

void SystemMessageGenerator::removed(const ChangeListener::element&) {}
