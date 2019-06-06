#ifndef EMAILADAPTER_H
#define EMAILADAPTER_H

#include <list>
#include <memory>

#include "email-agent/imapreciever.h"
#include "email-agent/smtpsender.h"
#include "interfaces/abstractchanneladapter.h"

class SmtpSender;
class ImapReciever;

class EmailAdapter : public AbstractChannelAdapter {
 public:
  using AbstractChannelAdapter::AbstractChannelAdapter;

 public:
  void send(const std::string& message,
            const std::string& adress) noexcept(false) override;
  std::pair<std::string, std::string> receive() noexcept(false) override;
  bool connect() override;

 private:
  std::list<std::pair<std::string, std::string> > mMessages;
  std::unique_ptr<SmtpSender> mSender;
  std::unique_ptr<ImapReciever> mReciever;
};

#endif  // EMAILADAPTER_H
