#include "oauthadapter.h"
#include "oauth-agents/utils/factories.h"

#include "oauth-agents/utils/base64.h"
#include "primitives/connectionholder.h"

#include <assert.h>
#include <thread>

void OauthAdapter::send(const std::string& message, const std::string& adress) {
  if (mOauthAgent->isExpired()) {
    mOauthAgent->refreshAccessToken();
  }

  auto [headerName, token] = mOauthAgent->getAuthParam();

  auto encoded =
      base64_encode(reinterpret_cast<const unsigned char*>(message.c_str()),
                    static_cast<unsigned int>(message.size()));

  mApiAgent->sendMessage(adress, encoded, headerName, token);
}

std::pair<std::string, std::string> OauthAdapter::receive() {
  if (mOauthAgent->isExpired()) {
    mOauthAgent->refreshAccessToken();
  }

  if (mMessages.empty()) {
    auto [headerName, token] = mOauthAgent->getAuthParam();
    auto list = mApiAgent->getMessages(headerName, token);
    mMessages.insert(mMessages.cend(), list.begin(), list.end());
  }

  if (!mMessages.empty()) {
    auto mess = mMessages.front();
    mess.second = base64_decode(mess.second);
    mMessages.pop_front();
    return mess;
  }

  std::this_thread::sleep_for(std::chrono::seconds(10));

  return std::pair<std::string, std::string>("", "");
}

bool OauthAdapter::connect(const ConnectionHolder& conn) {
  switch (conn.getType()) {
    case ConnectionType::GMAIL: {
      auto connInfo = conn.getConnection<GmailConnaection>();
      auto factory = makeFactory(AgentType::GMAIL);
      mOauthAgent = factory->makeOauthAgent(connInfo.accessToken);
      mApiAgent = factory->makeApiAgent(connInfo.email);
      return true;
    }
    case ConnectionType::VK: {
      auto connInfo = conn.getConnection<VkConnection>();
      auto factory = makeFactory(AgentType::VK);
      mOauthAgent = factory->makeOauthAgent(connInfo.accessToken);
      mApiAgent = factory->makeApiAgent(connInfo.userId);
      return true;
    }
    default:
      throw std::runtime_error(
          "Undefined connection type: " +
          std::to_string(static_cast<int>(conn.getType())));
  }
}

bool OauthAdapter::isConnected() const noexcept {
  return true;
}
