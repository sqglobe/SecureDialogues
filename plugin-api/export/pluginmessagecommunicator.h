#ifndef PLUGINMESSAGECOMMUNICATOR_H
#define PLUGINMESSAGECOMMUNICATOR_H

#include "export-definition.h"
#include "pluginapierrorcodes.h"

class RecievedMessagesIterator;
class PluginConnectionInfo;

class EXPORTED PluginMessageCommunicator {
 public:
  virtual ~PluginMessageCommunicator() = default;

 public:
  [[nodiscard]] virtual PluginApiErrorCodes send(
      const char* addressTo,
      const char* message) noexcept = 0;
  [[nodiscard]] virtual RecievedMessagesIterator* recieve() noexcept = 0;
  [[nodiscard]] virtual PluginApiErrorCodes connect(
      const PluginConnectionInfo* connInfo) noexcept = 0;
};

#endif  // PLUGINMESSAGECOMMUNICATOR_H
