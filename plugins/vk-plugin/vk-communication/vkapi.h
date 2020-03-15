#ifndef VKAPI_H
#define VKAPI_H

#include <list>
#include <tuple>
#include "httprequest.h"
class AuthFailException : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};
class VkApi {
 public:
  explicit VkApi(std::string userId);

 public:
  std::list<std::pair<std::string, std::string> > getMessages(
      const std::string& authHeaderName,
      const std::string& authToken) noexcept(false);

  void sendMessage(const std::string& to,
                   const std::string& body,
                   const std::string& authHeaderName,
                   const std::string& authToken) noexcept(false);

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
