#include "messageactionhandler.h"

#include "containers/messagecontainer.h"
#include "delivery-handlers/deliveryhandler.h"
#include "interfaces/abstractusernotifier.h"
#include "messagedespatcher.h"
#include "primitives/dialog.h"

#include "interfaces/cryptosystem.h"

#include "communication_helpers.h"
#include "containers/usermessage.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/spdlog.h"

#include <libintl.h>
#include <limits>
#include "fmt/core.h"

#undef ERROR

class MessageNotDeliveriedHandler : public DeliveryHandler {
 public:
  MessageNotDeliveriedHandler(
      std::unique_ptr<MessageContainer::Wrapper>&& wrapper) :
      mMessageWrapper(std::move(wrapper)) {}

 public:
  void removed() override;
  void timeouted() override;

 private:
  std::unique_ptr<MessageContainer::Wrapper> mMessageWrapper;
};

void MessageNotDeliveriedHandler::removed() {
  mMessageWrapper->get()->updateStatus(UserMessage::Status::Deliveried);
  mMessageWrapper->save();
}

void MessageNotDeliveriedHandler::timeouted() {
  mMessageWrapper->get()->updateStatus(UserMessage::Status::NotDeliveried);
  mMessageWrapper->save();
}

MessageActionHandler::MessageActionHandler(
    std::shared_ptr<DialogStorage> dialogs,
    std::shared_ptr<ContactStorage> contacts,
    std::shared_ptr<MessageContainer> container,
    std::shared_ptr<const MessageDespatcher> despatcher,
    std::shared_ptr<AbstractUserNotifier> notifier,
    std::shared_ptr<const CryptoSystem> system) :
    mDialogStorage(std::move(dialogs)),
    mContactStorage(std::move(contacts)),
    mMessageContainer(std::move(container)), mDespatcher(std::move(despatcher)),
    mNotifier(std::move(notifier)), mCryptoSystem(std::move(system)) {}

// TODO : make save for exceptions
void MessageActionHandler::handle(const DialogMessage& message,
                                  const std::string& channel) noexcept {
  try {
    if (!mDialogStorage->has(std::string(message.dialogId()))) {
      const std::string err =
          fmt::format(dgettext("messages",
                               "Recieved message from {}. With specified user "
                               "has no opened dialogs"),
                      message.adress());
      mNotifier->notify(AbstractUserNotifier::Severity::Error, err);
      if (auto despatcher = mDespatcher.lock()) {
        spdlog::get("root_logger")
            ->warn(
                "get message action from {0}, creatred dialog not found. Send "
                "abort",
                message.adress());
        despatcher->sendAndForget(
            make_abort(std::string(message.dialogId()),
                       std::string(message.adress()),
                       std::numeric_limits<unsigned long>::max()),
            channel);
      }
      return;
    }
    if (auto dialog = mDialogStorage->get(std::string(message.dialogId()));
        !dialog.isSequentalOk(message.sequential())) {
      spdlog::get("root_logger")
          ->warn(
              "Get message with invalid sequential {0}, dialog id{1}, action: "
              "{2}",
              message.sequential(), message.dialogId(),
              static_cast<int>(message.action()));
      return;
    }
    auto content = mCryptoSystem->decryptMessageBody(message.dialogId(),
                                                     message.content());
    mMessageContainer->addMessage(std::string(message.dialogId()), content,
                                  true);
  } catch (std::exception& ex) {
    abortOnException(message, channel);
    spdlog::get("root_logger")
        ->warn("catch exception {0} when prepare message from {1}", ex.what(),
               message.adress());
  }
}

bool MessageActionHandler::isItYouAction(DialogMessage::Action action) const
    noexcept {
  return DialogMessage::Action::MESSAGE == action;
}

void MessageActionHandler::sendMessage(std::string&& message) {
  std::string dialogId = mMessageContainer->getActiveDialog();
  if (dialogId.empty()) {
    mNotifier->notify(AbstractUserNotifier::Severity::Error,
                      dgettext("messages", "Select one dialog for texting"));
  }
  try {
    auto dialog = mDialogStorage->wrapper(dialogId);
    auto contact = mContactStorage->get(std::string(dialog->getContactId()));
    std::string textMess = message;
    auto [msg, channel] =
        make_text_message(dialog, contact, std::move(message));
    dialog.save();
    msg.setContent(
        mCryptoSystem->encryptMessageBody(msg.dialogId(), msg.content()));
    if (auto despatcher = mDespatcher.lock()) {
      despatcher->sendMessage(
          std::move(msg), channel,
          std::make_shared<MessageNotDeliveriedHandler>(
              mMessageContainer->addToActiveDialogWithWrapper(
                  std::move(textMess), false)));
    } else {
      mNotifier->notify(AbstractUserNotifier::Severity::Error,
                        dgettext("messages", "Failed to send message"));
    }
  } catch (const std::exception& ex) {
    mMessageContainer->addMessage(
        dialogId,
        std::make_shared<UserMessage>(
            UserMessage::Status::Error, UserMessage::Type::System,
            fmt::format(
                dgettext("messages", "Failed to send messsage, reason: {}"),
                ex.what())));
  }
}

void MessageActionHandler::setActiveDialog(const std::string& dialogId) {
  mMessageContainer->setActiveDialog(dialogId);
}

void MessageActionHandler::abortOnException(
    const DialogMessage& message,
    const std::string& channel) noexcept {
  try {
    mMessageContainer->addMessage(
        std::string(message.dialogId()),
        std::make_shared<UserMessage>(
            UserMessage::Status::Error, UserMessage::Type::System,
            fmt::format(dgettext("messages",
                                 "Error occured during message handling from "
                                 "user {}. Dialog removed."),
                        message.adress())));
    if (mDialogStorage->has(std::string(message.dialogId()))) {
      auto wrapper = mDialogStorage->wrapper(std::string(message.dialogId()));
      auto contact = mContactStorage->get(std::string(wrapper->getContactId()));
      auto mess = make_abort(std::string(wrapper->getDialogId()),
                             std::string(contact.adress()),
                             wrapper->makeNextSequental());
      wrapper->setStatus(Dialog::Status::ABORTED);
      if (auto despatcher = mDespatcher.lock()) {
        despatcher->sendAndForget(std::move(mess), channel);
      }
      wrapper.save();
    } else {
      if (auto despatcher = mDespatcher.lock()) {
        despatcher->sendAndForget(
            make_abort(std::string(message.dialogId()),
                       std::string(message.adress()),
                       std::numeric_limits<unsigned long>::max()),
            channel);
      }
    }
  } catch (std::exception& ex) {
    spdlog::get("root_logger")
        ->warn("catch exception {0} when abort message from {1}", ex.what(),
               message.adress());
  }
}
