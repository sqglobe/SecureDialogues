#ifndef PLUGINADAPTER_H
#define PLUGINADAPTER_H

#include "interfaces/abstractchanneladapter.h"
#include "pluginmessagecommunicatorwrapper.h"
class PluginAdapter : public AbstractChannelAdapter {
 public:
  PluginAdapter(
      std::unique_ptr<plugin_support::PluginMessageCommunicatorWrapper>&&
          pluginWrapper,
      std::shared_ptr<AbstractUserNotifier> notifier,
      ConnectionHolder conn);

 public:
  void send(const std::string& message,
            const std::string& adress) noexcept(false) override;
  std::list<std::pair<std::string, std::string>> receive() noexcept(
      false) override;
  bool connect() override;

 private:
  std::unique_ptr<plugin_support::PluginMessageCommunicatorWrapper>
      mPluginWrapper;
};

#endif  // PLUGINADAPTER_H
