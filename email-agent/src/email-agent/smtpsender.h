#ifndef SMTPSENDER_H
#define SMTPSENDER_H

#include <string>
#include <vmime/vmime.hpp>

class SmtpSender {
 public:
  SmtpSender(const std::string& address,
             const std::string& from,
             int port,
             bool tlsUsed,
             const std::string& userName,
             const std::string& pass,
             const std::string& subject);

 public:
  void connect();
  void send(const std::string& to, const std::string& body);

 private:
  std::string mFrom;
  std::string mSubject;

  vmime::shared_ptr<vmime::net::transport> mTransport;
};

#endif  // SMTPSENDER_H
