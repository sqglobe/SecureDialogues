#include "pluginadapter.h"

#include <export/recievedmessagesiterator.h>
#include <libintl.h>
#include <cassert>
#include "exception/disconectedexception.h"
#include "exception/notauthorizedexception.h"
#include "exception/receiveexception.h"
#include "exception/sendexception.h"
#include "fmt/core.h"

PluginAdapter::PluginAdapter(
    std::unique_ptr<plugin_support::PluginMessageCommunicatorWrapper>&&
        pluginWrapper,
    std::shared_ptr<AbstractUserNotifier> notifier,
    ConnectionHolder conn) :
    AbstractChannelAdapter(std::move(notifier), std::move(conn)),
    mPluginWrapper(std::move(pluginWrapper)) {}

void PluginAdapter::send(const std::string& message,
                         const std::string& adress) {
  const auto res = mPluginWrapper->send(adress, message);
  switch (res) {
    case PluginApiErrorCodes::NoError:
      return;
    case PluginApiErrorCodes::SendFailed:
      throw SendMessageException(fmt::format(
          dgettext("messages", "Failed to send message to: {}"), adress));
    case PluginApiErrorCodes::Disconected:
      throw DisconectedException(
          fmt::format(dgettext("messages", "Disconected from {}"), adress));
    case PluginApiErrorCodes::NotAuthorized:
      throw NotAuthorizedException("");
    case PluginApiErrorCodes::InvalidAddress:
      throw std::runtime_error(
          fmt::format(dgettext("messages", "Invalid address {}"), adress));
    default:
      assert(false);
  }
}

std::list<std::pair<std::string, std::string> > PluginAdapter::receive() {
  auto iter = mPluginWrapper->recieve();
  if (const auto err = iter->error(); err != PluginApiErrorCodes::NoError) {
    switch (err) {
      case PluginApiErrorCodes::RecieveFailed:
        throw ReceiveException(
            dgettext("messages", "Failed to recieve message"));
      case PluginApiErrorCodes::Disconected:
        throw DisconectedException("");
      case PluginApiErrorCodes::NotAuthorized:
        throw NotAuthorizedException("");
      default:
        assert(false);
    }
  }
  std::list<std::pair<std::string, std::string> > res;

  for (; iter->hasNext(); iter->next()) {
    res.emplace_back(std::make_pair(std::string(iter->address()),
                                    std::string(iter->message())));
  }
  return res;
}

bool PluginAdapter::connect() {
  auto res = mPluginWrapper->connect(getHolder().pluginConnInfo());
  return res == PluginApiErrorCodes::NoError;
}
