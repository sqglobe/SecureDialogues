#ifndef GMAILMESSAGECOMMUNICATOR_H
#define GMAILMESSAGECOMMUNICATOR_H

#include <memory>
#include "export/pluginmessagecommunicator.h"

#include "oauth-agents/interfaces/apiagent.h"
#include "oauth-agents/interfaces/oauthagent.h"

class GmailMessageCommunicator : public PluginMessageCommunicator {
 public:
  virtual PluginApiErrorCodes send(const char* addressTo,
                                   const char* message) noexcept override;
  virtual RecievedMessagesIterator* recieve() noexcept override;
  virtual PluginApiErrorCodes connect(
      const PluginConnectionInfo* connInfo) noexcept override;

 private:
  std::unique_ptr<OAuthAgent> mOauthAgent;
  std::unique_ptr<ApiAgent> mApiAgent;
};

#endif  // GMAILMESSAGECOMMUNICATOR_H
