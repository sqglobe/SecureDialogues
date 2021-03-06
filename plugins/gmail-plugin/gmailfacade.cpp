#include "gmailfacade.h"

#include "gmailconnectioninfo.h"
#include "gmailconnectionserializer.h"
#include "gmailconnectionwidget.h"
#include "gmailmessagecommunicator.h"
#include "gmailplugindefs.h"
#include "recievedmessagesiteratorimpl.h"

#include "gmailaddressvalidator.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"

GmailFacade::GmailFacade() :
    mWidget(std::make_unique<GmailConnectionWidget>()),
    mSerializer(std::make_unique<GmailConnectionSerializer>()),
    mValidator(std::make_unique<GmailAddressValidator>()) {
  spdlog::rotating_logger_mt("gmail_logger", "gmail-plugin.log", 1048576 * 5,
                             3);
}

GmailFacade::~GmailFacade() {}

const char* GmailFacade::getId() const noexcept {
  return ::getPluginId();
}

PluginWidget* GmailFacade::getWidget() const noexcept {
  return mWidget.get();
}

const PluginConnectionSerializer* GmailFacade::getSerializer() const noexcept {
  return mSerializer.get();
}

PluginMessageCommunicator* GmailFacade::makeCommunicator() noexcept {
  return new GmailMessageCommunicator;
}

PluginConnectionInfo* GmailFacade::makeEmptyConn() noexcept {
  return new GmailConnectionInfo;
}

const PluginAddressValidator* GmailFacade::getAddressValidator() const
    noexcept {
  return mValidator.get();
}

void GmailFacade::release(PluginMessageCommunicator* comm) noexcept {
  delete comm;
}

void GmailFacade::release(PluginConnectionInfo* connInfo) noexcept {
  delete connInfo;
}

void GmailFacade::release(RecievedMessagesIterator* iter) noexcept {
  delete iter;
}
