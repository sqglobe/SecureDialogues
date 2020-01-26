#ifndef PLUGINMESSAGECOMMUNICATORWRAPPER_H
#define PLUGINMESSAGECOMMUNICATORWRAPPER_H

#include <memory>
#include "export/pluginapierrorcodes.h"
#include "owneddeletior.h"

class PluginMessageCommunicator;
class RecievedMessagesIterator;
class PluginConnectionInfo;
class PlaginFacade;

namespace plugin_support {

class PluginInterface;

class PluginMessageCommunicatorWrapper {
 public:
  PluginMessageCommunicatorWrapper(
      std::unique_ptr<PluginMessageCommunicator, owned_deletor>&& communicator,
      std::shared_ptr<PluginInterface> interface,
      PlaginFacade* facade);

 public:
  [[nodiscard]] PluginApiErrorCodes send(const std::string& addressTo,
                                         const std::string& message) noexcept;
  [[nodiscard]] std::unique_ptr<RecievedMessagesIterator, owned_deletor>
  recieve() noexcept;
  [[nodiscard]] PluginApiErrorCodes connect(
      const std::shared_ptr<const PluginConnectionInfo>& connInfo) noexcept;

 private:
  std::unique_ptr<PluginMessageCommunicator, owned_deletor> mCommunicator;
  std::shared_ptr<PluginInterface> mInterface;
  PlaginFacade* mFacade;
};

}  // namespace plugin_support
#endif  // PLUGINMESSAGECOMMUNICATORWRAPPER_H
