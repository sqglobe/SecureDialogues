#include "dialogactiondeliveryhandler.h"
#include "interfaces/abstractmessagedespatcher.h"

#include "interfaces/abstractusernotifier.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/spdlog.h"

#undef ERROR

#include <libintl.h>
#include "fmt/core.h"

class DialogActionDeliveryHandler : public DeliveryHandler {
 public:
  DialogActionDeliveryHandler(
      std::weak_ptr<AbstractMessageDespatcher>&& dispatcher,
      DialogStorage::wrapper_type&& wrapper,
      Dialog::Status nextStatus,
      bool abortNeed,
      std::shared_ptr<AbstractUserNotifier>&& notifier,
      std::string&& address,
      std::string&& channel);

 public:
  void removed() override;
  void timeouted() override;

 protected:
  std::shared_ptr<AbstractUserNotifier> mNotifier;
  DialogStorage::wrapper_type mWrapper;
  std::weak_ptr<AbstractMessageDespatcher> mDispatcher;
  std::string mAddress;
  std::string mChannel;
  Dialog::Status mNext;
  bool mAbortNeed;
};

DialogActionDeliveryHandler::DialogActionDeliveryHandler(
    std::weak_ptr<AbstractMessageDespatcher>&& dispatcher,
    DialogStorage::wrapper_type&& wrapper,
    Dialog::Status nextStatus,
    bool abortNeed,
    std::shared_ptr<AbstractUserNotifier>&& notifier,
    std::string&& address,
    std::string&& channel) :
    mNotifier(std::move(notifier)),
    mWrapper(std::move(wrapper)), mDispatcher(std::move(dispatcher)),
    mAddress(std::move(address)), mChannel(std::move(channel)),
    mNext(nextStatus), mAbortNeed(abortNeed)

{}

void DialogActionDeliveryHandler::removed() {
  mWrapper.reload();
  mWrapper->setStatus(mNext);
  mWrapper.save();
}

void DialogActionDeliveryHandler::timeouted() {
  mWrapper.reload();
  if (mAbortNeed) {
    if (auto lock = mDispatcher.lock()) {
      spdlog::get("root_logger")
          ->warn(
              " ACK for dialog {0} and adress {1} not recieved. ABORT sended. "
              "Next {2}",
              mWrapper->getDialogId(), mAddress, static_cast<int>(mNext));

      lock->sendAndForget(
          make_abort(std::string(mWrapper->getDialogId()),
                     std::string(mAddress), mWrapper->makeNextSequental()),
          mChannel);
    }
  }
  mNotifier->notify(
      AbstractUserNotifier::Severity::ERROR,
      fmt::format(
          dgettext(
              "messages",
              "Dialog with {} was removed because participant didn't respond"),
          mAddress));
  mWrapper->setStatus(Dialog::Status::ABORTED);
  mWrapper.save();
}

std::shared_ptr<DeliveryHandler>
make_delivery_handler_for_create_dialog_request(
    std::weak_ptr<AbstractMessageDespatcher> dispatcher,
    DialogStorage::wrapper_type&& wrapper,
    std::shared_ptr<AbstractUserNotifier> notifier,
    std::string&& address,
    std::string&& channel) {
  return std::make_shared<DialogActionDeliveryHandler>(
      std::move(dispatcher), std::move(wrapper), Dialog::Status::WAIT_CONFIRM,
      false, std::move(notifier), std::move(address), std::move(channel));
}

std::shared_ptr<DeliveryHandler>
make_delivery_handler_for_wait_verification_dialog_request(
    std::weak_ptr<AbstractMessageDespatcher> dispatcher,
    DialogStorage::wrapper_type&& wrapper,
    std::shared_ptr<AbstractUserNotifier> notifier,
    std::string&& address,
    std::string&& channel) {
  return std::make_shared<DialogActionDeliveryHandler>(
      std::move(dispatcher), std::move(wrapper),
      Dialog::Status::WAIT_KEY_VERIFICAION, true, std::move(notifier),
      std::move(address), std::move(channel));
}

std::shared_ptr<DeliveryHandler> make_delivery_handler_for_close_dialog_request(
    std::weak_ptr<AbstractMessageDespatcher> dispatcher,
    DialogStorage::wrapper_type&& wrapper,
    std::shared_ptr<AbstractUserNotifier> notifier,
    std::string&& address,
    std::string&& channel) {
  return std::make_shared<DialogActionDeliveryHandler>(
      std::move(dispatcher), std::move(wrapper), Dialog::Status::CLOSED, false,
      std::move(notifier), std::move(address), std::move(channel));
}

std::shared_ptr<DeliveryHandler>
make_delivery_handler_for_active_dialog_request(
    std::weak_ptr<AbstractMessageDespatcher> dispatcher,
    DialogStorage::wrapper_type&& wrapper,
    std::shared_ptr<AbstractUserNotifier> notifier,
    std::string&& address,
    std::string&& channel) {
  return std::make_shared<DialogActionDeliveryHandler>(
      std::move(dispatcher), std::move(wrapper), Dialog::Status::ACTIVE, true,
      std::move(notifier), std::move(address), std::move(channel));
}
