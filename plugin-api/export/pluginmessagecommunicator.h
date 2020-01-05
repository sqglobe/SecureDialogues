#ifndef PLUGINMESSAGECOMMUNICATOR_H
#define PLUGINMESSAGECOMMUNICATOR_H

#include "pluginapierrorcodes.h"

class RecievedMessagesIterator;
class PluginConnectionInfo;

class PluginMessageCommunicator {
 public:
  virtual ~PluginMessageCommunicator() = default;

 public:
  virtual PluginApiErrorCodes send(const char* addressTo,
                                   const char* message) noexcept = 0;
  virtual RecievedMessagesIterator* recieve() noexcept = 0;
  virtual PluginApiErrorCodes connect(
      const PluginConnectionInfo* connInfo) noexcept = 0;
};

#endif  // PLUGINMESSAGECOMMUNICATOR_H
