#include "oauthadapter.h"
#include "oauth-agents/utils/factories.h"

#include <assert.h>
#include <thread>
#include "exception/disconectedexception.h"
#include "exception/notauthorizedexception.h"
#include "oauth-agents/exceptions/oauth-exceptions.h"
#include "oauth-agents/utils/base64.h"
#include "primitives/connectionholder.h"

#include "curlexceptions.h"

void OauthAdapter::send(const std::string& message, const std::string& adress) {
  try {
    if (mOauthAgent->isExpired()) {
      mOauthAgent->refreshAccessToken();
    }

    auto [headerName, token] = mOauthAgent->getAuthParam();

    auto encoded =
        base64_encode(reinterpret_cast<const unsigned char*>(message.c_str()),
                      static_cast<unsigned int>(message.size()));

    mApiAgent->sendMessage(adress, encoded, headerName, token);
  } catch (const AuthFailException& ex) {
    throw NotAuthorizedException(ex.what());
  } catch (const HttpError& error) {
    throw DisconectedException(error.what());
  } catch (const CurlHttpSendError& ex) {
    throw DisconectedException(ex.what());
  }
}

std::pair<std::string, std::string> OauthAdapter::receive() {
  try {
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
  } catch (const AuthFailException& ex) {
    throw NotAuthorizedException(ex.what());
  } catch (const HttpError& error) {
    throw DisconectedException(error.what());
  } catch (const CurlHttpSendError& ex) {
    throw DisconectedException(ex.what());
  }
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
