#ifndef DUMMYMESSAGECOMMUNICATOR_H
#define DUMMYMESSAGECOMMUNICATOR_H

#include "export/pluginmessagecommunicator.h"

class DummyMessageCommunicator final : public PluginMessageCommunicator {
 public:
  RecievedMessagesIterator* recieve() noexcept override;
  PluginApiErrorCodes connect(
      const PluginConnectionInfo* connInfo) noexcept override;
  PluginApiErrorCodes send(const char* addressTo,
                           const char* message) noexcept override;
};

#endif  // DUMMYMESSAGECOMMUNICATOR_H
