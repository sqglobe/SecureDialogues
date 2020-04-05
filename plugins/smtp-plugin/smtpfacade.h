#ifndef SMTPFACADE_H
#define SMTPFACADE_H

#include <memory>
#include "export/pluginfacade.h"

class SmtpConnectionWidget;
class SmtpConnectionSerializer;
class SmtpAddressValidator;

class SmtpFacade final : public PluginFacade {
 public:
  SmtpFacade();
  ~SmtpFacade() override;

 public:
  const char* getId() const noexcept override;

 public:
  PluginWidget* getWidget() const noexcept override;
  const PluginConnectionSerializer* getSerializer() const noexcept override;
  PluginMessageCommunicator* makeCommunicator() noexcept override;
  PluginConnectionInfo* makeEmptyConn() noexcept override;
  const PluginAddressValidator* getAddressValidator() const noexcept override;

 public:
  void release(PluginMessageCommunicator* comm) noexcept override;
  void release(PluginConnectionInfo* connInfo) noexcept override;
  void release(RecievedMessagesIterator* iter) noexcept override;

 private:
  std::unique_ptr<SmtpConnectionWidget> mWidget;
  std::unique_ptr<const SmtpConnectionSerializer> mSerializer;
  std::unique_ptr<const SmtpAddressValidator> mValidator;
};

#endif  // SMTPFACADE_H
