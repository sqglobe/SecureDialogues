#ifndef VKFACADE_H
#define VKFACADE_H

#include <memory>
#include "export/pluginfacade.h"

class VkConnectionSerializer;
class VkConnectionWidget;
class VkAddressValidator;

class VkFacade : public PluginFacade {
 public:
  VkFacade();
  ~VkFacade() override;

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
  std::unique_ptr<VkConnectionWidget> mWidget;
  std::unique_ptr<const VkConnectionSerializer> mSerializer;
  std::unique_ptr<const VkAddressValidator> mValidator;
};

#endif  // VKFACADE_H
