#include "gmailapi.h"

#include <algorithm>
#include <iostream>
#include <iterator>

#include <regex>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "utils/base64.h"

#include <nlohmann/json.hpp>
#include "uribuilder.h"

#include <spdlog/spdlog.h>

const int MAX_SECONDS_OLD = 120;

bool isMessageOld(long long messageTime) {
  auto timePt =
      std::chrono::system_clock::now() - std::chrono::seconds(MAX_SECONDS_OLD);
  return messageTime < std::chrono::duration_cast<std::chrono::milliseconds>(
                           timePt.time_since_epoch())
                           .count();
}

std::pair<std::string, std::string> GmailApi::getMail(
    const std::string& id,
    const std::string& authHeaderName,
    const std::string& authToken,
    const std::string& mail) const noexcept(false) {
  UriBuilder builder("https://www.googleapis.com/gmail/v1/users");
  builder.appendPath(mail).appendPath("messages").appendPath(id);
  auto [code, response] =
      mRequest.get(builder.getUri(), {{authHeaderName, authToken}});

  if (HttpCode::OK != code) {
    auto logger = spdlog::get("gmail_logger");
    logger->error(
        "Failed to send message for getMail with code: {}, response: {}",
        static_cast<int>(code), response);
    throw std::runtime_error("Failed to send message");
  }

  auto body = nlohmann::json::parse(response);

  auto playload = body.at("payload");
  auto headers = playload.at("headers");
  auto it = std::find_if(headers.cbegin(), headers.cend(), [](const auto& val) {
    return val.contains("name") && val.at("name") == "From";
  });
  if (!isMessageOld(
          std::stoll(static_cast<std::string>(body.at("internalDate")))) &&
      it != headers.cend()) {
    std::smatch sm;  // same as std::match_results<string::const_iterator>
                     // sm;
    std::string val = it->at("value");
    std::string toMail =
        std::regex_search(val, sm, std::regex("<([^>]+)>")) && !sm.empty()
            ? sm[1]
            : std::string(it->at("value"));
    std::string body = playload.at("body").at("size") == 0 ||
                               playload.at("mimeType") != "text/plain" ||
                               mail == toMail
                           ? ""
                           : base64_decode(playload.at("body").at("data"));
    return std::pair<std::string, std::string>(toMail, body);
  }
  return std::pair<std::string, std::string>("", "");
}

std::string getTimeFilter() {
  auto t = std::time(nullptr);
  std::stringstream buffer;
  buffer << std::put_time(std::gmtime(&t), "newer: %Y/%m/%d");
  return buffer.str();
}

GmailApi::GmailApi(std::string mail) : mUserMail(std::move(mail)) {}

void GmailApi::sendMessage(const std::string& to,
                           const std::string& body,
                           const std::string& authHeaderName,
                           const std::string& authToken) {
  std::stringstream ss;
  ss << "From: " << mUserMail << "\n"
     << "To: " << to << "\n"
     << "Subject: "
     << "sequre-dialogues"
     << "\n"
     << "\n"
     << body;

  auto messBody = ss.str();

  UriBuilder builder("https://www.googleapis.com/upload/gmail/v1/users");
  builder.appendPath(mUserMail)
      .appendPath("messages")
      .appendPath("send")
      .appendQuery("uploadType", "media");

  auto [code, response] =
      mRequest.post(builder.getUri(), messBody,
                    {{"Content-Type", "message/rfc822"},
                     {authHeaderName, authToken},
                     {"Content-Length", std::to_string(messBody.size())}});
  if (HttpCode::OK != code) {
    auto logger = spdlog::get("gmail_logger");
    logger->error(
        "Failed to send message for sendMessage with code: {}, response: {}",
        static_cast<int>(code), response);
    throw std::runtime_error("Failed to send message");
  }
}

std::string GmailApi::loadMessages(
    const std::string& authHeaderName,
    const std::string& authToken,
    const std::string& lastPageToken,
    std::list<std::pair<std::string, std::string>>* loadedMessages) const {
  UriBuilder builder("https://www.googleapis.com/gmail/v1/users");
  builder.appendPath(mUserMail)
      .appendPath("messages")
      .appendQuery("labelIds[0]", "INBOX");

  if (!lastPageToken.empty()) {
    builder.appendQuery("pageToken", lastPageToken);
  }

  builder.appendQuery("q",
                      getTimeFilter() + " " + "subject:(sequre-dialogues)");
  auto [code, response] =
      mRequest.get(builder.getUri(), {{authHeaderName, authToken}});

  if (HttpCode::OK != code) {
    auto logger = spdlog::get("gmail_logger");
    logger->error(
        "Failed to send message for loadMessages with code: {}, response: {}",
        static_cast<int>(code), response);
    throw std::runtime_error("Failed to send message");
  }

  auto body = nlohmann::json::parse(response);
  if (!body.contains("messages"))
    return std::string();
  auto messages = body.at("messages");
  auto size = mLastMessage.empty()
                  ? std::min(static_cast<std::size_t>(INIT_LOAD_MESSAGES),
                             messages.size())
                  : messages.size();
  bool breaked = false;
  for (std::size_t i = 0; i < size; i++) {
    if (auto id = messages.at(i).at("id"); id != mLastMessage) {
      loadedMessages->push_front(
          getMail(id, authHeaderName, authToken, mUserMail));
    } else {
      breaked = true;
      break;
    }
  }
  auto first = messages.empty() ? "" : messages.at(0).at("id");
  if (!breaked && !mLastMessage.empty()) {
    auto pageToken = body.at("nextPageToken");
    loadMessages(authHeaderName, authToken, pageToken, loadedMessages);
  }

  return first;
}

std::list<std::pair<std::string, std::string>> GmailApi::getMessages(
    const std::string& authHeaderName,
    const std::string& authToken) {
  std::list<std::pair<std::string, std::string>> loadedMessages;
  auto last = loadMessages(authHeaderName, authToken, "", &loadedMessages);

  if (!last.empty()) {
    mLastMessage = last;
  }

  return loadedMessages;
}
