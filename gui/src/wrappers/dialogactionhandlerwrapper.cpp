#include "dialogactionhandlerwrapper.h"
#include "communication/dialogactionhandler.h"
#include "interfaces/abstractusernotifier.h"
#include "primitives/contact.h"

DialogActionHandlerWrapper::DialogActionHandlerWrapper(
    std::shared_ptr<DialogActionHandler> handler,
    std::shared_ptr<AbstractUserNotifier> notifier,
    QObject* parent) :
    QObject(parent),
    mHandler(std::move(handler)), mNotifier(std::move(notifier)) {}

void DialogActionHandlerWrapper::createDialogAction(
    std::shared_ptr<const Contact> contact) {
  try {
    mHandler->createDialog(contact);
  } catch (const std::exception&) {
    mNotifier->notify(AbstractUserNotifier::Severity::ERROR,
                      "Не удалось создать диалог");
  }
}

void DialogActionHandlerWrapper::acceptDialogAction(std::string dialogId) {
  try {
    mHandler->acceptDialog(dialogId);
  } catch (const std::out_of_range&) {
    mNotifier->notify(AbstractUserNotifier::Severity::ERROR,
                      "Не удалось подтвердить создание диалога. Причина - "
                      "диалог не существует");
  } catch (const std::exception&) {
    mNotifier->notify(AbstractUserNotifier::Severity::ERROR,
                      "Ошибка при подтверждении создания диалога.");
  }
}

void DialogActionHandlerWrapper::closeDialogAction(std::string dialogId) {
  try {
    mHandler->closeDialog(dialogId);
  } catch (const std::out_of_range&) {
    mNotifier->notify(
        AbstractUserNotifier::Severity::ERROR,
        "Не удалось закрыть диалог. Причина - диалог не существует");
  } catch (const std::exception&) {
    mNotifier->notify(AbstractUserNotifier::Severity::ERROR,
                      "Ошибка при закрытии диалога.");
  }
}

void DialogActionHandlerWrapper::cancelDialogAction(std::string dialogId) {
  try {
    mHandler->cancelDialog(dialogId);
  } catch (const std::out_of_range&) {
    mNotifier->notify(
        AbstractUserNotifier::Severity::ERROR,
        "Не удалось отменить создание диалога. Причина - диалог не существует");
  } catch (const std::exception&) {
    mNotifier->notify(AbstractUserNotifier::Severity::ERROR,
                      "Ошибка при отмене создания диалога.");
  }
}

void DialogActionHandlerWrapper::abortDialogAction(std::string dialogId) {
  try {
    mHandler->abortDialog(dialogId);
  } catch (const std::out_of_range&) {
    mNotifier->notify(
        AbstractUserNotifier::Severity::ERROR,
        "Не удалось завершить диалог. Причина - диалог не существует");
  } catch (const std::exception&) {
    mNotifier->notify(AbstractUserNotifier::Severity::ERROR,
                      "Ошибка при завершении диалога.");
  }
}

void DialogActionHandlerWrapper::removeDialogAction(std::string dialogId) {
  try {
    mHandler->removeDialog(dialogId);
  } catch (const std::out_of_range&) {
    mNotifier->notify(
        AbstractUserNotifier::Severity::ERROR,
        "Не удалось удалить диалог. Причина - диалог не существует");
  } catch (const std::exception&) {
    mNotifier->notify(AbstractUserNotifier::Severity::ERROR,
                      "Ошибка при удалении диалога.");
  }
}
