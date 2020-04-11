#include "smtpfacade.h"
#include "export/recievedmessagesiterator.h"
#include "smtpaddressvalidator.h"
#include "smtpconnectioninfo.h"
#include "smtpconnectionserializer.h"
#include "smtpconnectionwidget.h"
#include "smtpmessagecommunication.h"
#include "smtpplugindefs.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"

SmtpFacade::SmtpFacade() :
    mWidget(std::make_unique<SmtpConnectionWidget>()),
    mSerializer(std::make_unique<SmtpConnectionSerializer>()),
    mValidator(std::make_unique<SmtpAddressValidator>()) {
  spdlog::rotating_logger_mt("smtp_logger", "gmail-plugin.log", 1048576 * 5, 3);
}

SmtpFacade::~SmtpFacade() {}

const char* SmtpFacade::getId() const noexcept {
  return ::getPluginId();
}

PluginWidget* SmtpFacade::getWidget() const noexcept {
  return mWidget.get();
}

const PluginConnectionSerializer* SmtpFacade::getSerializer() const noexcept {
  return mSerializer.get();
}

PluginMessageCommunicator* SmtpFacade::makeCommunicator() noexcept {
  return new SmtpMessageCommunication;
}

PluginConnectionInfo* SmtpFacade::makeEmptyConn() noexcept {
  return new SmtpConnectionInfo;
}

const PluginAddressValidator* SmtpFacade::getAddressValidator() const noexcept {
  return mValidator.get();
}

void SmtpFacade::release(PluginMessageCommunicator* comm) noexcept {
  delete comm;
}

void SmtpFacade::release(PluginConnectionInfo* connInfo) noexcept {
  delete connInfo;
}

void SmtpFacade::release(RecievedMessagesIterator* iter) noexcept {
  delete iter;
}
