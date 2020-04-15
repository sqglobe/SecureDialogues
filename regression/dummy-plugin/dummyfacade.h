#ifndef DUMMYFACADE_H
#define DUMMYFACADE_H
#include <memory>
#include "export/pluginfacade.h"

class DummyConnectionWidget;
class DummyAddressValidator;
class DummyConnectionSerializer;

class DummyFacade final : public PluginFacade {
 public:
  DummyFacade();

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
  std::unique_ptr<DummyConnectionWidget> mWidget;
  std::unique_ptr<const DummyConnectionSerializer> mSerializer;
  std::unique_ptr<const DummyAddressValidator> mValidator;
};

#endif  // DUMMYFACADE_H
