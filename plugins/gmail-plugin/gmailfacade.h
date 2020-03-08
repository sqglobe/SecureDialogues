#ifndef GMAILFACADE_H
#define GMAILFACADE_H

#include <memory>
#include "export/pluginfacade.h"

class GmailConnectionWidget;
class GmailConnectionSerializer;
class GmailAddressValidator;

class GmailFacade final : public PluginFacade {
 public:
  GmailFacade();
  ~GmailFacade() override;

 public:
  const char* getId() const noexcept override;
  const char* getPluginName() const noexcept override;
  const char* getTranslationFileName(Language lang) const noexcept override;
  const char* getGettextDomain() const noexcept override;

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
  std::unique_ptr<GmailConnectionWidget> mWidget;
  std::unique_ptr<const GmailConnectionSerializer> mSerializer;
  std::unique_ptr<const GmailAddressValidator> mValidator;
};

#endif  // GMAILFACADE_H
