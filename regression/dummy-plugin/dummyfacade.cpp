#include "dummyfacade.h"
#include "dummyaddressvalidator.h"
#include "dummyconnectioninfo.h"
#include "dummyconnectionserializer.h"
#include "dummyconnectionwidget.h"
#include "dummymessagecommunicator.h"
#include "dummyplugindefs.h"
#include "recievedmessagesiteratorimpl.h"
DummyFacade::DummyFacade() :
    mWidget(std::make_unique<DummyConnectionWidget>()),
    mSerializer(std::make_unique<DummyConnectionSerializer>()),
    mValidator(std::make_unique<DummyAddressValidator>()) {}

const char* DummyFacade::getId() const noexcept {
  return ::getPluginId();
}

PluginWidget* DummyFacade::getWidget() const noexcept {
  return mWidget.get();
}

const PluginConnectionSerializer* DummyFacade::getSerializer() const noexcept {
  return mSerializer.get();
}

PluginMessageCommunicator* DummyFacade::makeCommunicator() noexcept {
  return new DummyMessageCommunicator;
}

PluginConnectionInfo* DummyFacade::makeEmptyConn() noexcept {
  return new DummyConnectionInfo;
}

const PluginAddressValidator* DummyFacade::getAddressValidator() const
    noexcept {
  return mValidator.get();
}

void DummyFacade::release(PluginMessageCommunicator* comm) noexcept {
  delete comm;
}

void DummyFacade::release(PluginConnectionInfo* connInfo) noexcept {
  delete connInfo;
}

void DummyFacade::release(RecievedMessagesIterator* iter) noexcept {
  delete iter;
}
