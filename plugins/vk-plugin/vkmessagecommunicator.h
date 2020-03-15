#ifndef VKMESSAGECOMMUNICATOR_H
#define VKMESSAGECOMMUNICATOR_H
#include <memory>
#include "export/pluginmessagecommunicator.h"
class VkApi;
class VkOauth;
class VkMessageCommunicator : public PluginMessageCommunicator {
 public:
  VkMessageCommunicator();
  ~VkMessageCommunicator() override;

 public:
  PluginApiErrorCodes send(const char* addressTo,
                           const char* message) noexcept override;
  RecievedMessagesIterator* recieve() noexcept override;
  PluginApiErrorCodes connect(
      const PluginConnectionInfo* connInfo) noexcept override;

 private:
  std::unique_ptr<VkApi> mApiAgent;
  std::unique_ptr<VkOauth> mOauthAgent;
};

#endif  // VKMESSAGECOMMUNICATOR_H
