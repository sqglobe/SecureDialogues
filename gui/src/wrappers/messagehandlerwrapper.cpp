#include "messagehandlerwrapper.h"
#include "communication/messageactionhandler.h"
MessageHandlerWrapper::MessageHandlerWrapper(
    std::shared_ptr<MessageActionHandler> handler,
    QObject* parent) :
    QObject(parent),
    mMessageHandler(handler) {}

void MessageHandlerWrapper::setActiveDialog(std::string idDialog) {
  mMessageHandler->setActiveDialog(idDialog);
}

void MessageHandlerWrapper::cleareActiveDialog(std::string) {
  mMessageHandler->setActiveDialog("");
}

void MessageHandlerWrapper::sendMessage(std::string messageText) {
  mMessageHandler->sendMessage(messageText);
}
