#include "emailadapter.h"

#include "exception/disconectedexception.h"
#include "exception/notauthorizedexception.h"
#include "oauth-agents/utils/base64.h"
#include "primitives/connectionholder.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/spdlog.h"

static std::shared_ptr<spdlog::logger> LOGGER =
    spdlog::stdout_color_mt("email-adapter-logger");

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
      throw DisconectedException(ex.what());
    } catch (const vmime::exceptions::authentication_error& ex) {
      throw NotAuthorizedException(ex.what());
    } catch (const vmime::exceptions::connection_greeting_error& ex) {
      throw DisconectedException(ex.what());
    } catch (const vmime::exceptions::socket_not_connected_exception& ex) {
      throw DisconectedException(ex.what());
    }
  }

  throw std::runtime_error("Undefined connection type: " +
                           std::to_string(static_cast<int>(conn.getType())));
}
