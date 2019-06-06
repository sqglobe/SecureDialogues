#include "dialogactionhandler.h"
#include "containers/contactcontainer.h"
#include "containers/dialogmanager.h"

#include <cassert>
#include "delivery-handlers/dialogactiondeliveryhandler.h"
#include "interfaces/abstractmessagedespatcher.h"
#include "interfaces/abstractusernotifier.h"

#include "spdlog/spdlog.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include "interfaces/cryptosystem.h"

#undef ERROR

std::shared_ptr<spdlog::logger> DialogActionHandler::LOGGER =
    spdlog::stdout_color_mt("dialog-action-handler");

std::string get_dialog_status_description(Dialog::Status status) {
  using S = Dialog::Status;
  switch (status) {
    case S::ACTIVE:
      return "Пользователь подтвердил создание диалога. Тперь возможно "
             "пересылать ему сообщения";
    case S::CANCELED:
      return "Создание диалога было отменено удаленным пользователем. "
             "Поробуйте пересоздать диалог";
    case S::CLOSED:
      return "Диалог был закрыт";
    case S::NEW:
      return "Новый диалог";
    case S::ABORTED:
      return "Закрыт";
    case S::WAIT_CONFIRM:
      return "Ожидает решение пользователя на подтверждение или отказ от "
             "установления диалога";
    case S::ACCEPTED_WAIT:
      return "Ожидает подтверждения о получении согласия на создание диалога";
    case S::CANCEL_WAIT:
      return "Ожидает подтверждения о получении отказа в устанослении диалога";
    case S::CREATE_REQUEST:
      return "Пользователь запросил создание диалога";
    case S::WAIT_KEY_VERIFICAION:
      return "Ожидается подтверждение сеансового ключа";
    default:
      assert(false);
  }
}

std::string dialog_to_string(const std::shared_ptr<const Dialog>& dialog) {
  std::string res;
  auto status = dialog->getStatus();
  res.reserve(500);
  res.append(get_dialog_status_description(status));
  res.append(". Пользователь: ");
  res.append(dialog->getName());
  res.append(", Канал: ");
  res.append(dialog->getChannelMoniker());
  return res;
}

DialogActionHandler::DialogActionHandler(
    const std::shared_ptr<DialogManager>& dialogManager,
    const std::shared_ptr<AbstractMessageDespatcher>& dispatcher,
    const std::shared_ptr<AbstractUserNotifier>& notifier,
    const std::shared_ptr<ContactContainer>& contacts,
    const std::shared_ptr<CryptoSystem>& cryptoSystem) :
    mDialogManager(dialogManager),
    mMessageDispatcher(dispatcher), mNotifier(notifier),
    mContactContainer(contacts), mCryptoSystem(cryptoSystem) {}

void DialogActionHandler::handle(const DialogMessage& message,
                                 const std::string& channel) noexcept {
  try {
    if (!mDialogManager->has(message.dialogId())) {
      prepareNotFoundDialog(message, channel);
    } else {
      auto dialog = mDialogManager->get(message.dialogId());
      if (dialog->isSequentalOk(message.sequential())) {
        prepareForFoundDialog(message, channel);
      } else {
        LOGGER->warn(
            "Get message with invalid sequential {0}, dialog id{1}, action: "
            "{2}",
            message.sequential(), message.dialogId(),
            static_cast<int>(message.action()));
      }
    }
  } catch (std::exception& ex) {
    prepareHandleException(message, channel, ex);
  }
}

bool DialogActionHandler::isItYouAction(DialogMessage::Action action) const
    noexcept {
  using A = DialogMessage::Action;
  return A::ACCEPT_DIALOG == action || A::CANCEL_DIALOG == action ||
         A::CLOSE_DIALOG == action || A::CREATE_DIALOG == action ||
         A::ABORT == action || A::KEY_VERIFICATION == action ||
         A::VERIFY_KEY == action;
}

void DialogActionHandler::removeDialog(const std::string& dialogId) {
  closeDialog(dialogId);
  auto wrapper = mDialogManager->wrapper(dialogId);
  wrapper.remove();
}

void DialogActionHandler::createDialog(
    const std::shared_ptr<const Contact>& contact) {
  auto dialog = mDialogManager->make(contact);
  sendRequest(
      dialog, DialogMessage::Action::CREATE_DIALOG,
      make_delivery_handler_for_create_dialog_request(
          mMessageDispatcher, mDialogManager->wrapper(dialog), mNotifier));
}

void DialogActionHandler::acceptDialog(const std::string& dialogId) {
  auto dialog = mDialogManager->get(dialogId);
  auto message = dialog->makeMessage(
      DialogMessage::Action::VERIFY_KEY,
      mCryptoSystem->generateAndExportKey(dialogId, dialog->getAdress()));
  if (auto lock = mMessageDispatcher.lock()) {
    lock->sendMessage(
        message, dialog->getChannelMoniker(),
        make_delivery_handler_for_wait_verification_dialog_request(
            mMessageDispatcher, mDialogManager->wrapper(dialogId), mNotifier));
  } else {
    throw std::runtime_error("Не удалось отправить сообщение для контакта " +
                             dialog->getDialogId());
  }
}

void DialogActionHandler::abortDialog(const std::string& dialogId) {
  auto dialog = mDialogManager->wrapper(dialogId);
  if (auto lock = mMessageDispatcher.lock()) {
    if (dialog->isMessageActionAllowed(DialogMessage::Action::ABORT)) {
      lock->sendAndForget(dialog->makeAbort(), dialog->getChannelMoniker());
      dialog->setStatus(Dialog::Status::ABORTED);
      dialog.save();
    }
  }
}

void DialogActionHandler::closeDialog(const std::string& dialogId) {
  auto status = mDialogManager->get(dialogId)->getStatus();
  if (status == Dialog::Status::ACTIVE) {
    sendRequest(
        dialogId, DialogMessage::Action::CLOSE_DIALOG,
        make_delivery_handler_for_close_dialog_request(
            mMessageDispatcher, mDialogManager->wrapper(dialogId), mNotifier));
  } else {
    abortDialog(dialogId);
  }
}

void DialogActionHandler::cancelDialog(const std::string& dialogId) {
  sendRequest(
      dialogId, DialogMessage::Action::CANCEL_DIALOG,
      make_delivery_handler_for_cancel_dialog_request(
          mMessageDispatcher, mDialogManager->wrapper(dialogId), mNotifier));
}

/*
 * DialogActionDeliveryHandler(const std::shared_ptr<AbstractMessageDespatcher>
 &dispatcher, const  std::shared_ptr<Dialog> &dialog, const
 std::shared_ptr<DialogManager> &manager, Dialog::Status nextStatus);
 */

void DialogActionHandler::sendRequest(
    const std::string& dialogId,
    DialogMessage::Action action,
    const std::shared_ptr<DeliveryHandler>& handler,
    const std::string& content) {
  auto dialog = mDialogManager->get(dialogId);
  auto message = dialog->makeMessage(action, content);
  if (auto lock = mMessageDispatcher.lock()) {
    lock->sendMessage(message, dialog->getChannelMoniker(), handler);
  } else {
    throw std::runtime_error("Не удалось отправить сообщение для контакта " +
                             dialog->getDialogId());
  }
}

void DialogActionHandler::prepareNotFoundDialog(const DialogMessage& message,
                                                const std::string& channel) {
  using A = DialogMessage::Action;
  if (A::ABORT == message.action()) {
    return;
  }
  try {
    if (A::CREATE_DIALOG == message.action()) {
      auto contact = mContactContainer->getContactByAdress(message.adress());
      auto dialog = std::make_shared<Dialog>(contact, message.dialogId(),
                                             message.sequential(),
                                             Dialog::Status::CREATE_REQUEST);
      mDialogManager->add(dialog);
      mNotifier->notify(AbstractUserNotifier::Severity::INFO,
                        dialog_to_string(dialog));
      return;
    }
  } catch (std::range_error& ex) {
    mNotifier->notify(AbstractUserNotifier::Severity::ERROR,
                      "Адресат '" + message.adress() +
                          "' прислал запрос на добавление диалога, но контакт "
                          "с таким адрсом не найден");
  }
  if (auto lock = mMessageDispatcher.lock()) {
    lock->sendAndForget(make_abort(message.dialogId(), message.adress()),
                        channel);
    LOGGER->warn(
        "Get message with action {0} for dialog with adress {1} content {2}. "
        "This dialog is not found. Aborted",
        static_cast<int>(message.action()), message.adress(),
        message.content());
  }
}

void DialogActionHandler::prepareForFoundDialog(const DialogMessage& message,
                                                const std::string& channel) {
  if (message.action() == DialogMessage::Action::VERIFY_KEY) {
    auto res = mCryptoSystem->importKey(message.dialogId(), message.content());
    sendRequest(message.dialogId(), DialogMessage::Action::KEY_VERIFICATION,
                make_delivery_handler_for_create_dialog_request(
                    mMessageDispatcher,
                    mDialogManager->wrapper(message.dialogId()), mNotifier),
                res);
  } else if (message.action() == DialogMessage::Action::KEY_VERIFICATION) {
    if (mCryptoSystem->checkVerificationString(message.dialogId(),
                                               message.content())) {
      sendRequest(message.dialogId(), DialogMessage::Action::ACCEPT_DIALOG,
                  make_delivery_handler_for_active_dialog_request(
                      mMessageDispatcher,
                      mDialogManager->wrapper(message.dialogId()), mNotifier));
      LOGGER->debug("Get message KEY_VERIFICATION from {0}", channel);
    } else {
      sendRequest(message.dialogId(), DialogMessage::Action::CANCEL_DIALOG,
                  make_delivery_handler_for_cancel_dialog_request(
                      mMessageDispatcher,
                      mDialogManager->wrapper(message.dialogId()), mNotifier));
      mNotifier->notify(
          AbstractUserNotifier::Severity::INFO,
          "Не удалось произвести верификацию сеансового ключа с контактом " +
              message.adress() + " диалог завершен");
    }

  } else {
    auto dialog = mDialogManager->wrapper(message.dialogId());
    dialog->setStatus(map(message.action()));
    dialog.save();
    mNotifier->notify(AbstractUserNotifier::Severity::INFO,
                      dialog_to_string(dialog.get()));
  }
}

Dialog::Status DialogActionHandler::map(DialogMessage::Action action) const
    noexcept {
  using A = DialogMessage::Action;
  switch (action) {
    case A::ACCEPT_DIALOG:
      return Dialog::Status::ACTIVE;
    case A::CANCEL_DIALOG:
      return Dialog::Status::CANCELED;
    case A::CLOSE_DIALOG:
      return Dialog::Status::CLOSED;
    case A::ABORT:
      return Dialog::Status::ABORTED;
    default:
      assert(false);
  }
}

bool DialogActionHandler::isRemoveDialogAction(
    DialogMessage::Action action) const noexcept {
  using A = DialogMessage::Action;
  return action == A::CANCEL_DIALOG || action == A::CLOSE_DIALOG ||
         action == A::ABORT;
}

void DialogActionHandler::prepareHandleException(
    const DialogMessage& message,
    const std::string& channel,
    const std::exception& ex) noexcept {
  try {
    LOGGER->warn("dialog {0} from {1} makes error {2}", message.dialogId(),
                 channel, ex.what());
    if (mDialogManager->has(message.dialogId())) {
      abortDialog(message.dialogId());
    } else {
      if (auto lock = mMessageDispatcher.lock()) {
        lock->sendAndForget(make_abort(message.dialogId(), message.adress()),
                            channel);
      }
    }

  } catch (std::exception& exc) {
    LOGGER->error(
        "dialog {0} from {1} get excpetion {2} when prepare handle exception "
        "{3}",
        message.dialogId(), channel, exc.what(), ex.what());
  }
}
