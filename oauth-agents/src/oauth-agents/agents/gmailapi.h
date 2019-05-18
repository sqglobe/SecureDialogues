#ifndef GMAILAPI_H
#define GMAILAPI_H

#include "oauth-agents/interfaces/apiagent.h"

class GmailApi : public ApiAgent {
 public:
  explicit GmailApi(const std::string& mail);

 public:
  virtual std::list<std::pair<std::string, std::string> > getMessages(
      const std::string& authHeaderName,
      const std::string& authToken) noexcept(false) override;
  virtual void sendMessage(
      const std::string& to,
      const std::string& body,
      const std::string& authHeaderName,
      const std::string& authToken) noexcept(false) override;

 private:
  std::string loadMessages(
      const std::string& authHeaderName,
      const std::string& authToken,
      const std::string& lastPageToken,
      std::list<std::pair<std::string, std::string> >* loadedMessages) const
      noexcept(false);

 private:
  std::string mLastMessage;
  std::string mUserMail;
  static const int INIT_LOAD_MESSAGES = 20;
};

#endif  // GMAILAPI_H
