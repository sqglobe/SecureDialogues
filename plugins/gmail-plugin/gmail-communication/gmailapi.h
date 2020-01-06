#ifndef GMAILAPI_H
#define GMAILAPI_H

#include <list>
#include "httprequest.h"

class GmailApi {
 public:
  explicit GmailApi(std::string mail);

 public:
  std::list<std::pair<std::string, std::string> > getMessages(
      const std::string& authHeaderName,
      const std::string& authToken) noexcept(false);
  void sendMessage(const std::string& to,
                   const std::string& body,
                   const std::string& authHeaderName,
                   const std::string& authToken) noexcept(false);

 private:
  std::string loadMessages(
      const std::string& authHeaderName,
      const std::string& authToken,
      const std::string& lastPageToken,
      std::list<std::pair<std::string, std::string> >* loadedMessages) const
      noexcept(false);
  std::pair<std::string, std::string> getMail(const std::string& id,
                                              const std::string& authHeaderName,
                                              const std::string& authToken,
                                              const std::string& mail) const
      noexcept(false);

 private:
  std::string mLastMessage;
  std::string mUserMail;
  static const int INIT_LOAD_MESSAGES = 20;
  HttpRequest mRequest;
};

#endif  // GMAILAPI_H
