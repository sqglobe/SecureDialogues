#ifndef VKAPI_H
#define VKAPI_H

#include <tuple>
#include "httprequest.h"
#include "oauth-agents/interfaces/apiagent.h"

class VkApi : public ApiAgent {
 public:
  explicit VkApi(const std::string& userId);

 public:
  std::list<std::pair<std::string, std::string> > getMessages(
      const std::string& authHeaderName,
      const std::string& authToken) noexcept(false) override;

  void sendMessage(const std::string& to,
                   const std::string& body,
                   const std::string& authHeaderName,
                   const std::string& authToken) noexcept(false) override;

 private:
  std::tuple<std::string, std::string, int> getLongPollServer(
      const std::string& authHeaderName,
      const std::string& authToken);

 private:
  std::string mUserId;
  std::string mKey;
  std::string mServer;
  int mLastNumber;
  HttpRequest mRequest;
};

#endif  // VKAPI_H
