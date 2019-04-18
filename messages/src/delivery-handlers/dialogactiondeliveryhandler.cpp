#include "dialogactiondeliveryhandler.h"
#include "containers/containerelementwrapper.h"
#include "containers/dialogmanager.h"
#include "interfaces/abstractmessagedespatcher.h"

#include "interfaces/abstractusernotifier.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/spdlog.h"

static std::shared_ptr<spdlog::logger> LOGGER =
    spdlog::stdout_color_mt("dialog-action-delivery-handler");

class DialogActionDeliveryHandler : public DeliveryHandler {
 public:
  DialogActionDeliveryHandler(
      const std::weak_ptr<AbstractMessageDespatcher>& dispatcher,
      DialogManager::wrapper_type&& wrapper,
      Dialog::Status nextStatus,
      bool abortNeed,
      const std::shared_ptr<AbstractUserNotifier>& notifier);

 public:
  virtual void removed() override;
  virtual void timeouted() override;

 protected:
  std::shared_ptr<AbstractUserNotifier> mNotifier;
  DialogManager::wrapper_type mWrapper;
  std::weak_ptr<AbstractMessageDespatcher> mDispatcher;
  Dialog::Status mNext;
  bool mAbortNeed;
};

DialogActionDeliveryHandler::DialogActionDeliveryHandler(
    const std::weak_ptr<AbstractMessageDespatcher>& dispatcher,
    DialogManager::wrapper_type&& wrapper,
    Dialog::Status nextStatus,
    bool abortNeed,
    const std::shared_ptr<AbstractUserNotifier>& notifier) :
    mNotifier(notifier),
    mWrapper(std::move(wrapper)), mDispatcher(dispatcher), mNext(nextStatus),
    mAbortNeed(abortNeed)

{}

void DialogActionDeliveryHandler::removed() {
  mWrapper->setStatus(mNext);
  mWrapper.save();
}

void DialogActionDeliveryHandler::timeouted() {
  if (mAbortNeed) {
    if (auto lock = mDispatcher.lock()) {
      LOGGER->warn(
          " ACK for dialog {0} and adress {1} not recieved. ABORT sended ",
          mWrapper->getDialogId(), mWrapper->getAdress());
      lock->sendAndForget(mWrapper->makeAbort(), mWrapper->getChannelMoniker());
    }
  }
  mNotifier->notify(AbstractUserNotifier::Severity::ERROR,
                    "Диалог для " + mWrapper->getAdress() +
                        " удален, потому что удаленная сторона не отвечает");
  mWrapper->setStatus(Dialog::Status::ABORTED);
  mWrapper.save();
}

std::shared_ptr<DeliveryHandler>
make_delivery_handler_for_create_dialog_request(
    const std::weak_ptr<AbstractMessageDespatcher>& dispatcher,
    DialogManager::wrapper_type&& wrapper,
    const std::shared_ptr<AbstractUserNotifier>& notifier) {
  return std::make_shared<DialogActionDeliveryHandler>(
      dispatcher, std::move(wrapper), Dialog::Status::WAIT_CONFIRM, false,
      notifier);
}

std::shared_ptr<DeliveryHandler>
make_delivery_handler_for_cancel_dialog_request(
    const std::weak_ptr<AbstractMessageDespatcher>& dispatcher,
    DialogManager::wrapper_type&& wrapper,
    const std::shared_ptr<AbstractUserNotifier>& notifier) {
  return std::make_shared<DialogActionDeliveryHandler>(
      dispatcher, std::move(wrapper), Dialog::Status::CANCELED, true, notifier);
}

std::shared_ptr<DeliveryHandler>
make_delivery_handler_for_wait_verification_dialog_request(
    const std::weak_ptr<AbstractMessageDespatcher>& dispatcher,
    DialogManager::wrapper_type&& wrapper,
    const std::shared_ptr<AbstractUserNotifier>& notifier) {
  return std::make_shared<DialogActionDeliveryHandler>(
      dispatcher, std::move(wrapper), Dialog::Status::WAIT_KEY_VERIFICAION,
      true, notifier);
}

std::shared_ptr<DeliveryHandler> make_delivery_handler_for_close_dialog_request(
    const std::weak_ptr<AbstractMessageDespatcher>& dispatcher,
    DialogManager::wrapper_type&& wrapper,
    const std::shared_ptr<AbstractUserNotifier>& notifier) {
  return std::make_shared<DialogActionDeliveryHandler>(
      dispatcher, std::move(wrapper), Dialog::Status::CLOSED, false, notifier);
}

std::shared_ptr<DeliveryHandler>
make_delivery_handler_for_active_dialog_request(
    const std::weak_ptr<AbstractMessageDespatcher>& dispatcher,
    DialogManager::wrapper_type&& wrapper,
    const std::shared_ptr<AbstractUserNotifier>& notifier) {
  return std::make_shared<DialogActionDeliveryHandler>(
      dispatcher, std::move(wrapper), Dialog::Status::ACTIVE, true, notifier);
}
