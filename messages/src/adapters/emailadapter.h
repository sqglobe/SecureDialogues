#ifndef EMAILADAPTER_H
#define EMAILADAPTER_H

#include <list>
#include <memory>

#include "email-agent/imapreciever.h"
#include "email-agent/smtpsender.h"
#include "interfaces/abstractchanneladapter.h"

class SmtpSender;
class ImapReciever;

class EmailAdapter {
 public:
 public:
  void send(const std::string& message,
            const std::string& adress) noexcept(false);
  std::pair<std::string, std::string> receive() noexcept(false);
  bool connect();

 private:
  std::list<std::pair<std::string, std::string> > mMessages;
  std::unique_ptr<SmtpSender> mSender;
  std::unique_ptr<ImapReciever> mReciever;
};

#endif  // EMAILADAPTER_H
