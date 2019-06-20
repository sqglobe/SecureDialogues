#include "emailadapter.h"

#include "exception/disconectedexception.h"
#include "exception/notauthorizedexception.h"
#include "oauth-agents/utils/base64.h"
#include "primitives/connectionholder.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/spdlog.h"

void EmailAdapter::send(const std::string& message, const std::string& adress) {
  try {
    auto encoded =
        base64_encode(reinterpret_cast<const unsigned char*>(message.c_str()),
                      static_cast<unsigned int>(message.size()));
    mSender->send(adress, encoded);
  } catch (const vmime::exceptions::connection_error& ex) {
    throw DisconectedException(ex.what());
  } catch (const vmime::exceptions::socket_not_connected_exception& ex) {
    throw DisconectedException(ex.what());
  } catch (const vmime::exceptions::not_connected& ex) {
    throw DisconectedException(ex.what());
  }
}

std::pair<std::string, std::string> EmailAdapter::receive() {
  try {
    if (mMessages.empty()) {
      auto list = mReciever->recievedMessages();
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
  } catch (const vmime::exceptions::connection_error& ex) {
    throw DisconectedException(ex.what());
  } catch (const vmime::exceptions::socket_not_connected_exception& ex) {
    throw DisconectedException(ex.what());
  } catch (const vmime::exceptions::not_connected& ex) {
    // TODO: need to handle transport not_connected
    throw DisconectedException(ex.what());
  }
}

bool EmailAdapter::connect() {
  auto conn = getHolder();
  if (ConnectionType::EMAIL == conn.getType()) {
    try {
      auto emailConn = conn.getConnection<EmailConnection>();
      mSender = std::make_unique<SmtpSender>(
          emailConn.smtpAddr, emailConn.from, emailConn.smtpPort,
          emailConn.tlsUsed, emailConn.userName, emailConn.password,
          "sequre-dialogues");
      mSender->connect();
      mReciever = std::make_unique<ImapReciever>(
          emailConn.imapAddr, emailConn.imapPort, emailConn.tlsUsed, "INBOX",
          emailConn.userName, emailConn.password, "sequre-dialogues");
      mReciever->connect();
      return true;
    } catch (const vmime::exceptions::connection_error& ex) {
      auto logger = spdlog::get("root_logger");
      logger->error("Get connection_error : {0}", ex.what());
      while (auto o = ex.other()) {
        logger->error("Parent exception {0} {1}", o->name(), o->what());
      }
      throw DisconectedException(ex.what());
    } catch (const vmime::exceptions::authentication_error& ex) {
      auto logger = spdlog::get("root_logger");
      logger->error("Get authentication_error : {0}", ex.what());
      while (auto o = ex.other()) {
        logger->error("Parent exception {0} {1}", o->name(), o->what());
      }
      throw NotAuthorizedException(ex.what());
    } catch (const vmime::exceptions::connection_greeting_error& ex) {
      auto logger = spdlog::get("root_logger");
      logger->error("Get connection_greeting_error : {0}", ex.what());
      while (auto o = ex.other()) {
        logger->error("Parent exception {0} {1}", o->name(), o->what());
      }
      throw DisconectedException(ex.what());
    } catch (const vmime::exceptions::socket_not_connected_exception& ex) {
      auto logger = spdlog::get("root_logger");
      logger->error("Get socket_not_connected_exception : {0}", ex.what());
      while (auto o = ex.other()) {
        logger->error("Parent exception {0} {1}", o->name(), o->what());
      }
      throw DisconectedException(ex.what());
    }
  }

  throw std::runtime_error("Undefined connection type: " +
                           std::to_string(static_cast<int>(conn.getType())));
}
