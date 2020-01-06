#include "gmailfacade.h"

#include "gmailconnectioninfo.h"
#include "gmailconnectionserializer.h"
#include "gmailconnectionwidget.h"
#include "gmailmessagecommunicator.h"
#include "gmailplugindefs.h"

#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"

GmailFacade::GmailFacade() :
    mWidget(std::make_unique<GmailConnectionWidget>()),
    mSerializer(std::make_unique<GmailConnectionSerializer>()) {
  spdlog::rotating_logger_mt("gmail_logger", "gmail-plugin.log", 1048576 * 5,
                             3);
}

GmailFacade::~GmailFacade() {}

const char* GmailFacade::getId() const noexcept {
  return ::getPluginId();
}

const char* GmailFacade::getPluginName() const noexcept {
  return "Gmail";
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

void GmailFacade::release(PluginMessageCommunicator* comm) noexcept {
  delete comm;
}

void GmailFacade::release(PluginConnectionInfo* connInfo) noexcept {
  delete connInfo;
}
