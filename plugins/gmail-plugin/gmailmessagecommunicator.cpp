#include "gmailmessagecommunicator.h"
#include "gmailconnectioninfo.h"
#include "recievedmessagesiteratorimpl.h"
#include "utils/base64.h"

#include <cassert>
#include <cstring>

#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/spdlog.h"

#include "curlexceptions.h"
#include "gmail-communication/gmailapi.h"
#include "gmail-communication/gmailoauth.h"
GmailMessageCommunicator::GmailMessageCommunicator() {}

GmailMessageCommunicator::~GmailMessageCommunicator() {}

PluginApiErrorCodes GmailMessageCommunicator::send(
    const char* addressTo,
    const char* message) noexcept {
  assert(mOauthAgent);
  assert(mApiAgent);
  try {
    if (mOauthAgent->isExpired()) {
      mOauthAgent->refreshAccessToken();
    }

    auto [headerName, token] = mOauthAgent->getAuthParam();

    auto encoded =
        base64_encode(reinterpret_cast<const unsigned char*>(message),
                      static_cast<unsigned int>(std::strlen(message)));

    mApiAgent->sendMessage(addressTo, encoded, headerName, token);
  } catch (const CurlHttpSendError& ex) {
    auto logger = spdlog::get("gmail_logger");
    logger->warn("HttpError {0} for message: {1}, address {2}", ex.what(),
                 message, addressTo);
    return PluginApiErrorCodes::Disconected;
  } catch (const std::exception& ex) {
    auto logger = spdlog::get("gmail_logger");
    logger->warn("std::exception {0} for message: {1}, address {2}", ex.what(),
                 message, addressTo);
    return PluginApiErrorCodes::SendFailed;
  }

  return PluginApiErrorCodes::NoError;
}

RecievedMessagesIterator* GmailMessageCommunicator::recieve() noexcept {
  assert(mOauthAgent);
  assert(mApiAgent);
  try {
    if (mOauthAgent->isExpired()) {
      mOauthAgent->refreshAccessToken();
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));

    auto [headerName, token] = mOauthAgent->getAuthParam();
    return new RecievedMessagesIteratorImpl(
        mApiAgent->getMessages(headerName, token));
  } catch (const CurlHttpSendError& ex) {
    auto logger = spdlog::get("root_logger");
    logger->warn("CurlHttpSendError {0} when try to recieve message",
                 ex.what());
    return new RecievedMessagesIteratorImpl(PluginApiErrorCodes::Disconected);
  } catch (const std::exception& ex) {
    auto logger = spdlog::get("root_logger");
    logger->warn("std::exception {0} when try to recieve message", ex.what());
    return new RecievedMessagesIteratorImpl(PluginApiErrorCodes::RecieveFailed);
  }
}

PluginApiErrorCodes GmailMessageCommunicator::connect(
    const PluginConnectionInfo* connInfo) noexcept {
  if (auto* conn = dynamic_cast<const GmailConnectionInfo*>(connInfo);
      conn == nullptr) {
    return PluginApiErrorCodes::Disconected;
  } else {
    try {
      mOauthAgent = std::make_unique<GmailOauth>(
          GOOGLE_CLIENT_ID, GOOGLE_CLIENT_SECRET, REDIRECT_URL);
      mApiAgent = std::make_unique<GmailApi>(conn->email);
      return PluginApiErrorCodes::NoError;
    } catch (const std::exception& ex) {
      auto logger = spdlog::get("root_logger");
      logger->warn("std::exception {0} when try to connect", ex.what());
      return PluginApiErrorCodes::Disconected;
    }
  }
}
