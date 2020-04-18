#ifndef SMTPMESSAGECOMMUNICATION_H
#define SMTPMESSAGECOMMUNICATION_H

#include <list>
#include <memory>
#include "export/pluginmessagecommunicator.h"

class SmtpSender;
class ImapReciever;

class SmtpMessageCommunication final : public PluginMessageCommunicator {
 public:
  SmtpMessageCommunication();
  ~SmtpMessageCommunication() override;

 public:
  virtual PluginApiErrorCodes send(const char* addressTo,
                                   const char* message) noexcept override;
  virtual RecievedMessagesIterator* recieve() noexcept override;
  virtual PluginApiErrorCodes connect(
      const PluginConnectionInfo* connInfo) noexcept override;

 private:
  std::unique_ptr<SmtpSender> mSender;
  std::unique_ptr<ImapReciever> mReciever;
};

#endif  // SMTPMESSAGECOMMUNICATION_H
