#ifndef GMAILFACADE_H
#define GMAILFACADE_H

#include <memory>
#include "export/plaginfacade.h"

class GmailConnectionWidget;
class GmailConnectionSerializer;

class GmailFacade final : public PlaginFacade {
 public:
  GmailFacade();
  ~GmailFacade() override;

 public:
  const char* getId() const noexcept override;
  const char* getPluginName() const noexcept override;

 public:
  PluginWidget* getWidget() const noexcept override;
  const PluginConnectionSerializer* getSerializer() const noexcept override;
  PluginMessageCommunicator* makeCommunicator() noexcept override;
  PluginConnectionInfo* makeEmptyConn() noexcept override;

 public:
  void release(PluginMessageCommunicator* comm) noexcept override;
  void release(PluginConnectionInfo* connInfo) noexcept override;
  void release(RecievedMessagesIterator* iter) noexcept override;

 private:
  std::unique_ptr<GmailConnectionWidget> mWidget;
  std::unique_ptr<const GmailConnectionSerializer> mSerializer;
};

#endif  // GMAILFACADE_H
