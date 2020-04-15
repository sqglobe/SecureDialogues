#include "dummymessagecommunicator.h"
#include "recievedmessagesiteratorimpl.h"

RecievedMessagesIterator* DummyMessageCommunicator::recieve() noexcept {
  return new RecievedMessagesIteratorImpl(PluginApiErrorCodes::NoError);
}

PluginApiErrorCodes DummyMessageCommunicator::connect(
    const PluginConnectionInfo*) noexcept {
  return PluginApiErrorCodes::NoError;
}

PluginApiErrorCodes DummyMessageCommunicator::send(const char*,
                                                   const char*) noexcept {
  return PluginApiErrorCodes::NoError;
}
