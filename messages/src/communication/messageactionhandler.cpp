#include "messageactionhandler.h"

#include "containers/dialogmanager.h"
#include "containers/messagecontainer.h"
#include "delivery-handlers/deliveryhandler.h"
#include "interfaces/abstractusernotifier.h"
#include "messagedespatcher.h"
#include "primitives/dialog.h"

#include "interfaces/cryptosystem.h"

#include "containers/usermessage.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/spdlog.h"

std::shared_ptr<spdlog::logger> MessageActionHandler::LOGGER =
    spdlog::stdout_color_mt("message-action-handler");

class MessageNotDeliveriedHandler : public DeliveryHandler {
 public:
  MessageNotDeliveriedHandler(
      std::unique_ptr<MessageContainer::Wrapper>&& wrapper) :
      mMessageWrapper(std::move(wrapper)) {}

 public:
  virtual void removed() override;
  virtual void timeouted() override;

 private:
  std::unique_ptr<MessageContainer::Wrapper> mMessageWrapper;
};

void MessageNotDeliveriedHandler::removed() {
  mMessageWrapper->get()->updateStatus(UserMessage::Status::DELIVERIED);
  mMessageWrapper->save();
}

void MessageNotDeliveriedHandler::timeouted() {
  mMessageWrapper->get()->updateStatus(UserMessage::Status::NOT_DELIVERIED);
  mMessageWrapper->save();
}

MessageActionHandler::MessageActionHandler(
    const std::shared_ptr<DialogManager>& manager,
    const std::shared_ptr<MessageContainer>& container,
    const std::shared_ptr<const MessageDespatcher>& despatcher,
    const std::shared_ptr<AbstractUserNotifier>& notifier,
    const std::shared_ptr<const CryptoSystem>& system) :
    mDialogManager(manager),
    mMessageContainer(container), mDespatcher(despatcher), mNotifier(notifier),
    mCryptoSystem(system) {}

// TODO : make save for exceptions
void MessageActionHandler::handle(const DialogMessage& message,
                                  const std::string& channel) noexcept {
  try {
    if (!mDialogManager->has(message.dialogId())) {
      std::string err("Получено сообщение от ");
      err.append(message.adress())
          .append(". С указанным пользователем отсутсвуют открытые диалоги");
      mNotifier->notify(AbstractUserNotifier::Severity::ERROR, err);
      if (auto despatcher = mDespatcher.lock()) {
        LOGGER->warn(
            "get message action from {0}, creatred dialog not found. Send "
            "abort",
            message.adress());
        despatcher->sendAndForget(
            make_abort(message.dialogId(), message.adress()), channel);
      }
    }
    auto content = mCryptoSystem->decryptMessageBody(message.dialogId(),
                                                     message.content());
    mMessageContainer->addMessage(message.dialogId(), content, true);
  } catch (std::exception& ex) {
    abortOnException(message, channel);
    LOGGER->warn("catch exception {0} when prepare message from {1}", ex.what(),
                 message.adress());
  }
}

bool MessageActionHandler::isItYouAction(DialogMessage::Action action) const
    noexcept {
  return DialogMessage::Action::MESSAGE == action;
}

void MessageActionHandler::sendMessage(const std::string& message) {
  std::string dialogId = mMessageContainer->getActiveDialog();
  if (dialogId.empty()) {
    mNotifier->notify(AbstractUserNotifier::Severity::ERROR,
                      "Выберите один диалог для отправки сообщения");
  }
  try {
    auto dialog = mDialogManager->get(dialogId);
    auto msg = dialog->makeMessage(DialogMessage::Action::MESSAGE, message);
    msg.setContent(
        mCryptoSystem->encryptMessageBody(msg.dialogId(), msg.content()));
    if (auto despatcher = mDespatcher.lock()) {
      despatcher->sendMessage(
          msg, dialog->getChannelMoniker(),
          std::make_shared<MessageNotDeliveriedHandler>(
              mMessageContainer->addToActiveDialogWithWrapper(message, false)));
    } else {
      mNotifier->notify(AbstractUserNotifier::Severity::ERROR,
                        "Не удалось отправить сообщение");
    }
  } catch (const std::exception& ex) {
    mMessageContainer->addMessage(
        dialogId,
        std::make_shared<UserMessage>(
            UserMessage::Status::ERROR, UserMessage::Type::SYSTEM,
            std::string("Не удалось отправить сообщение: ") + ex.what()));
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
        message.dialogId(),
        std::make_shared<UserMessage>(
            UserMessage::Status::ERROR, UserMessage::Type::SYSTEM,
            "При обработке сообщения от пользователя " + message.adress() +
                " произошла ошибка. Диалог удален"));
    if (mDialogManager->has(message.dialogId())) {
      auto wrapper = mDialogManager->wrapper(message.dialogId());
      auto mess = wrapper->makeAbort();
      if (auto despatcher = mDespatcher.lock()) {
        despatcher->sendAndForget(mess, channel);
      }
      wrapper.save();
    } else {
      if (auto despatcher = mDespatcher.lock()) {
        despatcher->sendAndForget(
            make_abort(message.dialogId(), message.adress()), channel);
      }
    }
  } catch (std::exception& ex) {
    LOGGER->warn("catch exception {0} when abort message from {1}", ex.what(),
                 message.adress());
  }
}
