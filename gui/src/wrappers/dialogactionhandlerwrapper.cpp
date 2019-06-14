#include "dialogactionhandlerwrapper.h"
#include "communication/dialogactionhandler.h"
#include "primitives/contact.h"
DialogActionHandlerWrapper::DialogActionHandlerWrapper(
    std::shared_ptr<DialogActionHandler> handler,
    QObject* parent) :
    QObject(parent),
    mHandler(std::move(handler)) {}

void DialogActionHandlerWrapper::createDialogAction(
    std::shared_ptr<const Contact> contact) {
  mHandler->createDialog(contact);
}

void DialogActionHandlerWrapper::closeDialogAction(std::string dialogId) {
  mHandler->closeDialog(dialogId);
}

void DialogActionHandlerWrapper::abortDialogAction(std::string dialogId) {
  mHandler->abortDialog(dialogId);
}

void DialogActionHandlerWrapper::removeDialogAction(std::string dialogId) {
  mHandler->removeDialog(dialogId);
}
