#include "vkfacade.h"

#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/spdlog.h"

#include "recievedmessagesiteratorimpl.h"
#include "vkconnectioninfo.h"
#include "vkconnectionserializer.h"
#include "vkconnectionwidget.h"
#include "vkmessagecommunicator.h"
#include "vkplugindefs.h"

#include "vkaddressvalidator.h"

VkFacade::VkFacade() :
    mWidget(std::make_unique<VkConnectionWidget>()),
    mSerializer(std::make_unique<VkConnectionSerializer>()),
    mValidator(std::make_unique<VkAddressValidator>()) {
  spdlog::rotating_logger_mt("vk_logger", "vk-plugin.log", 1048576 * 5, 3);
}

VkFacade::~VkFacade() {}

const char* VkFacade::getId() const noexcept {
  return ::getPluginId();
}

PluginWidget* VkFacade::getWidget() const noexcept {
  return mWidget.get();
}

const PluginConnectionSerializer* VkFacade::getSerializer() const noexcept {
  return mSerializer.get();
}

PluginMessageCommunicator* VkFacade::makeCommunicator() noexcept {
  return new VkMessageCommunicator;
}

PluginConnectionInfo* VkFacade::makeEmptyConn() noexcept {
  return new VkConnectionInfo;
}

const PluginAddressValidator* VkFacade::getAddressValidator() const noexcept {
  return mValidator.get();
}

void VkFacade::release(PluginMessageCommunicator* comm) noexcept {
  delete comm;
}

void VkFacade::release(PluginConnectionInfo* connInfo) noexcept {
  delete connInfo;
}

void VkFacade::release(RecievedMessagesIterator* iter) noexcept {
  delete iter;
}
