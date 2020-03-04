#include "oauthadapter.h"
#include "oauth-agents/utils/factories.h"

#include <cassert>
#include <thread>

#include "exception/disconectedexception.h"
#include "exception/notauthorizedexception.h"
#include "oauth-agents/exceptions/oauth-exceptions.h"
#include "oauth-agents/utils/base64.h"
#include "primitives/connectionholder.h"

#include "curlexceptions.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/spdlog.h"

#include <libintl.h>
#include "fmt/core.h"

void OauthAdapter::send(const std::string& message, const std::string& adress) {
  assert(mOauthAgent);
  assert(mApiAgent);
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
    auto logger = spdlog::get("root_logger");
    logger->warn("AuthFailException {0} for message: {1}, address {2}",
                 ex.what(), message, adress);
    throw NotAuthorizedException(ex.what());
  } catch (const HttpError& error) {
    auto logger = spdlog::get("root_logger");
    logger->warn("HttpError {0} for message: {1}, address {2}", error.what(),
                 message, adress);
    throw DisconectedException(error.what());
  } catch (const CurlHttpSendError& ex) {
    auto logger = spdlog::get("root_logger");
    logger->warn("HttpError {0} for message: {1}, address {2}", ex.what(),
                 message, adress);
    throw DisconectedException(ex.what());
  } catch (const std::exception& ex) {
    auto logger = spdlog::get("root_logger");
    logger->warn("std::exception {0} for message: {1}, address {2}", ex.what(),
                 message, adress);
    throw;
  }
}

std::pair<std::string, std::string> OauthAdapter::receive() {
  assert(mOauthAgent);
  assert(mApiAgent);
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
    auto logger = spdlog::get("root_logger");
    logger->warn("AuthFailException {0} when try to recieve message",
                 ex.what());
    throw NotAuthorizedException(ex.what());
  } catch (const HttpError& error) {
    auto logger = spdlog::get("root_logger");
    logger->warn("HttpError {0} when try to recieve message", error.what());
    throw DisconectedException(error.what());
  } catch (const CurlHttpSendError& ex) {
    auto logger = spdlog::get("root_logger");
    logger->warn("CurlHttpSendError {0} when try to recieve message",
                 ex.what());
    throw DisconectedException(ex.what());
  } catch (const std::exception& ex) {
    auto logger = spdlog::get("root_logger");
    logger->warn("std::exception {0} when try to recieve message", ex.what());
    throw;
  }
}

bool OauthAdapter::connect() {
  /*auto conn = getHolder();
  switch (conn.getType()) {
    case ConnectionType::GMAIL: {
      auto connInfo = conn.getConnection<GmailConnection>();
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
          fmt::format(dgettext("messages", "Undefined connection type: {}"),
                      static_cast<int>(conn.getType())));
  }*/
}
