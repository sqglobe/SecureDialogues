#ifndef GMAILFACADE_H
#define GMAILFACADE_H

#include <memory>
#include "export/plaginfacade.h"

class GmailConnectionWidget;
class GmailConnectionSerializer;

class GmailFacade : public PlaginFacade {
 public:
  GmailFacade();
  ~GmailFacade() override;

 public:
  virtual const char* getId() const noexcept override;
  virtual const char* getPluginName() const noexcept override;

 public:
  virtual PluginWidget* getWidget() const noexcept override;
  virtual const PluginConnectionSerializer* getSerializer() const
      noexcept override;
  virtual PluginMessageCommunicator* makeCommunicator() noexcept override;

 public:
  virtual void release(PluginMessageCommunicator* comm) noexcept override;
  virtual void release(PluginConnectionInfo* connInfo) noexcept override;

 private:
  std::unique_ptr<GmailConnectionWidget> mWidget;
  std::unique_ptr<const GmailConnectionSerializer> mSerializer;
};

#endif  // GMAILFACADE_H
