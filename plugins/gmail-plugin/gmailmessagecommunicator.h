#ifndef GMAILMESSAGECOMMUNICATOR_H
#define GMAILMESSAGECOMMUNICATOR_H

#include <memory>
#include "export/pluginmessagecommunicator.h"

class GmailOauth;
class GmailApi;
class GmailMessageCommunicator final : public PluginMessageCommunicator {
 public:
  GmailMessageCommunicator();
  ~GmailMessageCommunicator() override;

 public:
  virtual PluginApiErrorCodes send(const char* addressTo,
                                   const char* message) noexcept override;
  virtual RecievedMessagesIterator* recieve() noexcept override;
  virtual PluginApiErrorCodes connect(
      const PluginConnectionInfo* connInfo) noexcept override;

 private:
  std::unique_ptr<GmailOauth> mOauthAgent;
  std::unique_ptr<GmailApi> mApiAgent;
};

#endif  // GMAILMESSAGECOMMUNICATOR_H
