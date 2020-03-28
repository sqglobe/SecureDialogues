#include "smtpmessagecommunication.h"
#include <spdlog/spdlog.h>
#include <cstring>
#include "email-agent/imapreciever.h"
#include "email-agent/smtpsender.h"
#include "recievedmessagesiteratorimpl.h"
#include "smtpconnectioninfo.h"
#include "utils/base64.h"

SmtpMessageCommunication::SmtpMessageCommunication() {}

SmtpMessageCommunication::~SmtpMessageCommunication() {}

PluginApiErrorCodes SmtpMessageCommunication::send(
    const char* addressTo,
    const char* message) noexcept {
  try {
    auto encoded =
        base64_encode(reinterpret_cast<const unsigned char*>(message),
                      static_cast<unsigned int>(std::strlen(message)));
    mSender->send(addressTo, encoded);
    return PluginApiErrorCodes::NoError;
  } catch (const vmime::exceptions::connection_error& ex) {
    auto logger = spdlog::get("smtp_logger");
    logger->error("Failed send to {}. Reason - disconnected: {}", addressTo,
                  ex.what());
    return PluginApiErrorCodes::Disconected;
  } catch (const vmime::exceptions::socket_not_connected_exception& ex) {
    auto logger = spdlog::get("smtp_logger");
    logger->error(
        "Failed send to {}. Reason - socket_not_connected_exception: {}",
        addressTo, ex.what());
    return PluginApiErrorCodes::Disconected;
  } catch (const vmime::exceptions::not_connected& ex) {
    auto logger = spdlog::get("smtp_logger");
    logger->error("Failed send to {}. Reason - not_connected: {}", addressTo,
                  ex.what());
    return PluginApiErrorCodes::Disconected;
  }
}

RecievedMessagesIterator* SmtpMessageCommunication::recieve() noexcept {
  try {
    auto ptr = std::make_unique<RecievedMessagesIteratorImpl>(
        mReciever->recievedMessages());
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return ptr.release();
  } catch (const vmime::exceptions::connection_error& ex) {
    auto logger = spdlog::get("smtp_logger");
    logger->error("Failed to recieve messages. Reason - disconnected: {}",
                  ex.what());
    return new RecievedMessagesIteratorImpl(PluginApiErrorCodes::Disconected);
  } catch (const vmime::exceptions::socket_not_connected_exception& ex) {
    auto logger = spdlog::get("smtp_logger");
    logger->error(
        "Failed to recieve messages. Reason - socket_not_connected_exception: "
        "{}",
        ex.what());
    return new RecievedMessagesIteratorImpl(PluginApiErrorCodes::Disconected);
  } catch (const vmime::exceptions::not_connected& ex) {
    // TODO: need to handle transport not_connected
    auto logger = spdlog::get("smtp_logger");
    logger->error("Failed to recieve messages. Reason - not_connected: {}",
                  ex.what());
    return new RecievedMessagesIteratorImpl(PluginApiErrorCodes::Disconected);
  }
}

PluginApiErrorCodes SmtpMessageCommunication::connect(
    const PluginConnectionInfo* connInfo) noexcept {
  if (auto emailConn = dynamic_cast<const SmtpConnectionInfo*>(connInfo)) {
    try {
      mSender = std::make_unique<SmtpSender>(
          emailConn->smtpAddr, emailConn->from, emailConn->smtpPort,
          emailConn->tlsUsed, emailConn->userName, emailConn->password,
          "sequre-dialogues");
      mSender->connect();
      mReciever = std::make_unique<ImapReciever>(
          emailConn->imapAddr, emailConn->imapPort, emailConn->tlsUsed, "INBOX",
          emailConn->userName, emailConn->password, "sequre-dialogues");
      mReciever->connect();
      return PluginApiErrorCodes::NoError;
    } catch (const vmime::exceptions::connection_error& ex) {
      auto logger = spdlog::get("root_logger");
      logger->error("Get connection_error : {0}", ex.what());
      while (auto o = ex.other()) {
        logger->error("Parent exception {0} {1}", o->name(), o->what());
      }
      return PluginApiErrorCodes::Disconected;
    } catch (const vmime::exceptions::authentication_error& ex) {
      auto logger = spdlog::get("root_logger");
      logger->error("Get authentication_error : {0}", ex.what());
      while (auto o = ex.other()) {
        logger->error("Parent exception {0} {1}", o->name(), o->what());
      }
      return PluginApiErrorCodes::NotAuthorized;
    } catch (const vmime::exceptions::connection_greeting_error& ex) {
      auto logger = spdlog::get("root_logger");
      logger->error("Get connection_greeting_error : {0}", ex.what());
      while (auto o = ex.other()) {
        logger->error("Parent exception {0} {1}", o->name(), o->what());
      }
      return PluginApiErrorCodes::Disconected;
    } catch (const vmime::exceptions::socket_not_connected_exception& ex) {
      auto logger = spdlog::get("root_logger");
      logger->error("Get socket_not_connected_exception : {0}", ex.what());
      while (auto o = ex.other()) {
        logger->error("Parent exception {0} {1}", o->name(), o->what());
      }
      return PluginApiErrorCodes::Disconected;
    }
  }
  return PluginApiErrorCodes::NoError;
}
