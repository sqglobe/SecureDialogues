#ifndef APIAGENT_H
#define APIAGENT_H

#include <list>
#include <stdexcept>
#include <string>

class AuthFailException : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class ApiAgent {
 public:
  virtual ~ApiAgent() = default;
  virtual std::list<std::pair<std::string, std::string> > getMessages(
      const std::string& authHeaderName,
      const std::string& authToken) noexcept(false) = 0;
  virtual void sendMessage(const std::string& to,
                           const std::string& body,
                           const std::string& authHeaderName,
                           const std::string& authToken) noexcept(false) = 0;
};

#endif  // APIAGENT_H
