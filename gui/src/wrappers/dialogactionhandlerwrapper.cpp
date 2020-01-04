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

void DialogActionHandlerWrapper::createDialogAction(Contact contact) {
  try {
    mHandler->createDialog(contact);
  } catch (const std::exception&) {
    mNotifier->notify(AbstractUserNotifier::Severity::Error,
                      tr("Failed to create dialog").toStdString());
  }
}

void DialogActionHandlerWrapper::closeDialogAction(std::string dialogId) {
  try {
    mHandler->closeDialog(dialogId);
  } catch (const std::out_of_range&) {
    mNotifier->notify(
        AbstractUserNotifier::Severity::Error,
        tr("Failed to close dialog. Reason - dialog not presented")
            .toStdString());
  } catch (const std::exception&) {
    mNotifier->notify(AbstractUserNotifier::Severity::Error,
                      tr("Error occured during dialog closing").toStdString());
  }
}

void DialogActionHandlerWrapper::abortDialogAction(std::string dialogId) {
  try {
    mHandler->abortDialog(dialogId);
  } catch (const std::out_of_range&) {
    mNotifier->notify(
        AbstractUserNotifier::Severity::Error,
        tr("Failed to abort dialog. Reason - dialog not presented")
            .toStdString());
  } catch (const std::exception&) {
    mNotifier->notify(AbstractUserNotifier::Severity::Error,
                      tr("Error occured during dialog aborting").toStdString());
  }
}

void DialogActionHandlerWrapper::removeDialogAction(std::string dialogId) {
  try {
    mHandler->removeDialog(dialogId);
  } catch (const std::out_of_range&) {
    mNotifier->notify(
        AbstractUserNotifier::Severity::Error,
        tr("Failed to remove dialog. Reason - dialog not presented")
            .toStdString());
  } catch (const std::exception&) {
    mNotifier->notify(AbstractUserNotifier::Severity::Error,
                      tr("Error occured during dialog removing").toStdString());
  }
}
