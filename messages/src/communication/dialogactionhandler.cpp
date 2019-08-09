#include "dialogactionhandler.h"

#include <cassert>
#include "delivery-handlers/dialogactiondeliveryhandler.h"
#include "interfaces/abstractmessagedespatcher.h"
#include "interfaces/abstractusernotifier.h"

#include "spdlog/spdlog.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include "communication_helpers.h"
#include "interfaces/cryptosystem.h"
#undef ERROR

std::string get_dialog_status_description(Dialog::Status status) {
  using S = Dialog::Status;
  switch (status) {
    case S::ACTIVE:
      return "Пользователь подтвердил создание диалога. Тперь возможно "
             "пересылать ему сообщения";
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
    case S::CREATE_REQUEST:
      return "Пользователь запросил создание диалога";
    case S::WAIT_KEY_VERIFICAION:
      return "Ожидается подтверждение сеансового ключа";
    default:
      assert(false);
  }
}

std::string dialog_to_string(const Dialog& dialog, const Contact& contact) {
  std::string res;
  auto status = dialog.getStatus();
  res.reserve(500);
  res.append(get_dialog_status_description(status));
  res.append(". Пользователь: ");
  res.append(contact.name());
  res.append(", Канал: ");
  res.append(contact.channelMoniker());
  return res;
}

DialogActionHandler::DialogActionHandler(
    std::shared_ptr<DialogStorage> dialogManager,
    std::shared_ptr<AbstractMessageDespatcher> dispatcher,
    std::shared_ptr<AbstractUserNotifier> notifier,
    std::shared_ptr<ContactStorage> contacts,
    std::shared_ptr<CryptoSystem> cryptoSystem) :
    mDialogStorage(std::move(dialogManager)),
    mMessageDispatcher(std::move(dispatcher)), mNotifier(std::move(notifier)),
    mContactStorage(std::move(contacts)),
    mCryptoSystem(std::move(cryptoSystem)) {}

void DialogActionHandler::handle(const DialogMessage& message,
                                 const std::string& channel) noexcept {
  try {
    if (!mDialogStorage->has(std::string(message.dialogId()))) {
      prepareNotFoundDialog(message, channel);
    } else {
      auto dialog = mDialogStorage->wrapper(std::string(message.dialogId()));
      if (dialog->isSequentalOk(message.sequential())) {
        dialog.save();
        prepareForFoundDialog(message, channel);
      } else {
        spdlog::get("root_logger")
            ->warn(
                "Get message with invalid sequential {0}, dialog id{1}, "
                "action: "
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
  return A::ACCEPT_DIALOG == action || A::CLOSE_DIALOG == action ||
         A::CREATE_DIALOG == action || A::ABORT == action ||
         A::KEY_VERIFICATION == action || A::VERIFY_KEY == action;
}

void DialogActionHandler::removeDialog(std::string_view dialogId) {
  closeDialog(dialogId);
  auto wrapper = mDialogStorage->wrapper(std::string(dialogId));
  wrapper.remove();
}

void DialogActionHandler::createDialog(const Contact& contact) {
  auto dialog = Dialog(std::string(contact.id()));
  mDialogStorage->add(dialog);
  auto deliveryHandler = make_delivery_handler_for_create_dialog_request(
      mMessageDispatcher,
      mDialogStorage->wrapper(std::string(dialog.getDialogId())), mNotifier,
      std::string(contact.adress()), std::string(contact.channelMoniker()));
  sendRequest(std::string(dialog.getDialogId()),
              DialogMessage::Action::CREATE_DIALOG, std::move(deliveryHandler));
}

void DialogActionHandler::abortDialog(std::string_view dialogId) {
  auto dialog = mDialogStorage->wrapper(std::string(dialogId));
  auto contact = mContactStorage->get(std::string(dialog->getContactId()));
  if (auto lock = mMessageDispatcher.lock()) {
    if (dialog->isMessageActionAllowed(DialogMessage::Action::ABORT)) {
      auto mess = make_abort(std::string(dialog->getDialogId()),
                             std::string(contact.adress()),
                             dialog->makeNextSequental());
      lock->sendAndForget(std::move(mess), contact.channelMoniker());
      dialog->setStatus(Dialog::Status::ABORTED);
      dialog.save();
    }
  }
}

void DialogActionHandler::closeDialog(std::string_view dialogId) {
  auto wrapper = mDialogStorage->wrapper(std::string(dialogId));
  auto status = wrapper->getStatus();
  if (status == Dialog::Status::ACTIVE) {
    auto contact = mContactStorage->get(std::string(wrapper->getContactId()));
    auto handler = make_delivery_handler_for_close_dialog_request(
        mMessageDispatcher, std::move(wrapper), mNotifier,
        std::string(contact.adress()), std::string(contact.channelMoniker()));
    sendRequest(std::string(dialogId), DialogMessage::Action::CLOSE_DIALOG,
                std::move(handler));
  } else {
    abortDialog(dialogId);
  }
}

/*
 * DialogActionDeliveryHandler(const std::shared_ptr<AbstractMessageDespatcher>
 &dispatcher, const  std::shared_ptr<Dialog> &dialog, const
 std::shared_ptr<DialogManager> &manager, Dialog::Status nextStatus);
 */

void DialogActionHandler::sendRequest(
    const std::string& dialogId,
    DialogMessage::Action action,
    std::shared_ptr<DeliveryHandler>&& handler,
    std::string&& content) {
  auto dialog = mDialogStorage->wrapper(dialogId);
  auto contact = mContactStorage->get(std::string(dialog->getContactId()));
  auto [msg, channel] =
      make_message(action, dialog, contact, std::move(content));
  if (auto lock = mMessageDispatcher.lock()) {
    lock->sendMessage(std::move(msg), channel, std::move(handler));
    dialog.save();
  } else {
    throw std::runtime_error("Не удалось отправить сообщение для контакта " +
                             std::string(dialog->getDialogId()));
  }
}

void DialogActionHandler::prepareNotFoundDialog(const DialogMessage& message,
                                                const std::string& channel) {
  using A = DialogMessage::Action;
  if (A::ABORT == message.action()) {
    return;
  }

  if (A::CREATE_DIALOG == message.action()) {
    auto contacts =
        mContactStorage->get_if([address = message.adress()](
                                    const ContactStorage::element& el) -> bool {
          return el.adress() == address;
        });
    if (contacts.empty()) {
      mNotifier->notify(
          AbstractUserNotifier::Severity::ERROR,
          "Адресат '" + std::string(message.adress()) +
              "' прислал запрос на добавление диалога, но контакт "
              "с таким адрсом не найден");
    } else {
      auto contact = contacts.front();
      auto dialog =
          Dialog(std::string(contact.id()), std::string(message.dialogId()),
                 message.sequential(), Dialog::Status::CREATE_REQUEST);
      mDialogStorage->add(dialog);
      auto wrapper = mDialogStorage->wrapper(std::string(message.dialogId()));
      auto [msg, channel] =
          make_message(DialogMessage::Action::VERIFY_KEY, wrapper, contact,
                       mCryptoSystem->generateAndExportKey(message.dialogId(),
                                                           contact.adress()));

      if (auto lock = mMessageDispatcher.lock()) {
        auto handler =
            make_delivery_handler_for_wait_verification_dialog_request(
                mMessageDispatcher, std::move(wrapper), mNotifier,
                std::string(contact.adress()),
                std::string(contact.channelMoniker()));
        lock->sendMessage(std::move(msg), channel, std::move(handler));
      } else {
        mNotifier->notify(AbstractUserNotifier::Severity::ERROR,
                          "Не удалось отправить сообщение для контакта " +
                              std::string(dialog.getDialogId()));
      }
      return;
    }
  }

  if (auto lock = mMessageDispatcher.lock()) {
    lock->sendAndForget(make_abort(std::string(message.dialogId()),
                                   std::string(message.adress()),
                                   std::numeric_limits<unsigned long>::max()),
                        channel);
    spdlog::get("root_logger")
        ->warn(
            "Get message with action {0} for dialog with adress {1} content "
            "{2}. "
            "This dialog is not found. Aborted",
            static_cast<int>(message.action()), message.adress(),
            message.content());
  }
}

void DialogActionHandler::prepareForFoundDialog(const DialogMessage& message,
                                                const std::string& channel) {
  if (message.action() == DialogMessage::Action::VERIFY_KEY) {
    auto contact =
        mContactStorage->get(std::string(std::string(message.dialogId())));
    auto res = mCryptoSystem->importKey(message.dialogId(), message.content());
    auto handler = make_delivery_handler_for_create_dialog_request(
        mMessageDispatcher,
        mDialogStorage->wrapper(std::string(message.dialogId())), mNotifier,
        std::string(contact.adress()), std::string(contact.channelMoniker()));
    sendRequest(std::string(message.dialogId()),
                DialogMessage::Action::KEY_VERIFICATION, std::move(handler),
                std::move(res));
  } else if (message.action() == DialogMessage::Action::KEY_VERIFICATION) {
    if (mCryptoSystem->checkVerificationString(message.dialogId(),
                                               message.content())) {
      auto contact =
          mContactStorage->get(std::string(std::string(message.dialogId())));
      auto handler = make_delivery_handler_for_active_dialog_request(
          mMessageDispatcher,
          mDialogStorage->wrapper(std::string(message.dialogId())), mNotifier,
          std::string(contact.adress()), std::string(contact.channelMoniker()));
      sendRequest(std::string(message.dialogId()),
                  DialogMessage::Action::ACCEPT_DIALOG, std::move(handler));
    } else {
      abortDialog(message.dialogId());
      mNotifier->notify(
          AbstractUserNotifier::Severity::INFO,
          "Не удалось произвести верификацию сеансового ключа с контактом " +
              std::string(message.adress()) + " диалог завершен");
    }

  } else {
    auto dialog = mDialogStorage->wrapper(std::string(message.dialogId()));
    auto contact = mContactStorage->get(std::string(dialog->getDialogId()));
    dialog->setStatus(map(message.action()));
    dialog.save();
    mNotifier->notify(AbstractUserNotifier::Severity::INFO,
                      dialog_to_string(dialog.get(), contact));
  }
}

Dialog::Status DialogActionHandler::map(DialogMessage::Action action) const
    noexcept {
  using A = DialogMessage::Action;
  switch (action) {
    case A::ACCEPT_DIALOG:
      return Dialog::Status::ACTIVE;
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
  return action == A::CLOSE_DIALOG || action == A::ABORT;
}

void DialogActionHandler::prepareHandleException(
    const DialogMessage& message,
    const std::string& channel,
    const std::exception& ex) noexcept {
  try {
    spdlog::get("root_logger")
        ->warn("dialog {0} from {1} makes error {2}", message.dialogId(),
               channel, ex.what());
    if (mDialogStorage->has(std::string(message.dialogId()))) {
      abortDialog(message.dialogId());
    } else {
      if (auto lock = mMessageDispatcher.lock()) {
        lock->sendAndForget(
            make_abort(std::string(message.dialogId()),
                       std::string(message.adress()),
                       std::numeric_limits<unsigned long>::max()),
            channel);
      }
    }

  } catch (std::exception& exc) {
    spdlog::get("root_logger")
        ->error(
            "dialog {0} from {1} get excpetion {2} when prepare handle "
            "exception "
            "{3}",
            message.dialogId(), channel, exc.what(), ex.what());
  }
}
