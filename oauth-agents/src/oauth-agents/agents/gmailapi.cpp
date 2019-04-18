#include "gmailapi.h"

#include <cpprest/http_client.h>
#include "oauth-agents/exceptions/httpfailexception.h"

#include <algorithm>
#include <iterator>

#include <regex>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "oauth-agents/utils/base64.h"

const int MAX_SECONDS_OLD = 120;

bool isMessageOld(const web::json::value& val) {
  auto timePt =
      std::chrono::system_clock::now() - std::chrono::seconds(MAX_SECONDS_OLD);
  auto messageTime = std::stol(val.at(U("internalDate")).as_string());
  return messageTime < std::chrono::duration_cast<std::chrono::milliseconds>(
                           timePt.time_since_epoch())
                           .count();
}

pplx::task<std::pair<std::string, std::string>> getMail(
    const std::string& id,
    const std::string& authHeaderName,
    const std::string& authToken,
    const std::string& mail) {
  using namespace web::http::client;  // HTTP client features
  web::uri_builder builder;
  builder.append_path(mail).append_path("messages").append_path(id);

  http_client client(U("https://www.googleapis.com/gmail/v1/users"));
  web::http::http_request request(web::http::methods::GET);
  request.set_request_uri(builder.to_string());
  request.headers().add(authHeaderName, authToken);

  auto task =
      client.request(request)
          .then([&builder](web::http::http_response response) {
            if (web::http::status_codes::OK != response.status_code()) {
              throw HttpFailException(
                  response.status_code(),
                  "https://www.googleapis.com/gmail/v1/users" +
                      builder.to_string(),
                  response.extract_string().get());
            }
            return response.extract_json();
          })
          .then([&mail](web::json::value val) {
            try {
              auto playload = val.at("payload");
              auto headers = playload.at(U("headers")).as_array();
              auto it =
                  std::find_if(headers.cbegin(), headers.cend(),
                               [](const web::json::value& val) {
                                 return val.has_field("name") &&
                                        val.at("name").as_string() == "From";
                               });
              if (!isMessageOld(val) && it != headers.cend()) {
                std::smatch
                    sm;  // same as std::match_results<string::const_iterator>
                         // sm;
                std::string toMail =
                    std::regex_search(it->at("value").as_string(), sm,
                                      std::regex("<([^>]+)>")) &&
                            sm.size() > 0
                        ? sm[1]
                        : it->at("value").as_string();
                std::string body =
                    playload.at("body").at("size").as_integer() == 0 ||
                            playload.at("mimeType").as_string() !=
                                "text/plain" ||
                            mail == toMail
                        ? ""
                        : base64_decode(playload.at("body")
                                            .as_object()
                                            .at("data")
                                            .as_string());
                return std::pair<std::string, std::string>(toMail, body);
              }

            } catch (std::exception& ex) {
              std::cout << "Catch exception " << ex.what()
                        << " when load user mail " << std::endl;
            }
            return std::pair<std::string, std::string>("", "");
          });
  return task;
}

std::string getTimeFilter() {
  auto t = std::time(nullptr);
  std::stringstream buffer;
  buffer << std::put_time(std::gmtime(&t), "newer: %Y/%m/%d");
  return buffer.str();
}

GmailApi::GmailApi(const std::string& mail) : mUserMail(mail) {}

void GmailApi::sendMessage(const std::string& to,
                           const std::string& body,
                           const std::string& authHeaderName,
                           const std::string& authToken) {
  std::stringstream ss;
  ss << "From: " << mUserMail << "\n"
     << "To: " << to << "\n"
     << "Subject: "
     << "message-sender"
     << "\n"
     << "\n"
     << body;

  auto messBody = ss.str();

  using namespace web::http::client;  // HTTP client features
  web::uri_builder builder;
  builder.append_path(mUserMail)
      .append_path("messages")
      .append_path("send")
      .append_query("uploadType", "media");

  http_client client(U("https://www.googleapis.com/upload/gmail/v1/users"));

  web::http::http_request request(web::http::methods::POST);
  request.set_request_uri(builder.to_string());
  request.headers().add(authHeaderName, authToken);
  request.headers().add("Content-Type", "message/rfc822");
  request.headers().add("Content-Length", messBody.size());

  request.set_body(messBody);

  auto task = client.request(request).then(
      [&builder](web::http::http_response response) {
        if (web::http::status_codes::OK != response.status_code()) {
          throw HttpFailException(
              response.status_code(),
              " https://www.googleapis.com/upload/gmail/v1/users" +
                  builder.to_string(),
              response.extract_string().get());
        }
        return response.extract_json();
      });
  task.wait();
}

std::string GmailApi::loadMessages(
    const std::string& authHeaderName,
    const std::string& authToken,
    const std::string& lastPageToken,
    std::list<std::pair<std::string, std::string>>* loadedMessages) const {
  using namespace web::http::client;  // HTTP client features
  web::uri_builder builder;
  builder.append_path(mUserMail).append_path("messages");
  builder.append_query("labelIds[0]", "INBOX");

  if (!lastPageToken.empty()) {
    builder.append_query("pageToken", lastPageToken);
  }

  builder.append_query("q", getTimeFilter() + " " + "subject:(message-sender)");

  http_client client(U("https://www.googleapis.com/gmail/v1/users"));
  web::http::http_request request(web::http::methods::GET);
  request.set_request_uri(builder.to_string());
  request.headers().add(authHeaderName, authToken);

  auto task =
      client.request(request)
          .then([&builder](web::http::http_response response) {
            if (web::http::status_codes::OK != response.status_code()) {
              throw HttpFailException(
                  response.status_code(),
                  "https://www.googleapis.com/gmail/v1/users" +
                      builder.to_string(),
                  response.extract_string().get());
            }
            return response.extract_json();
          })
          .then([this, &authHeaderName, &authToken, loadedMessages,
                 &lastPageToken](web::json::value val) {
            try {
              if (!val.has_field("messages"))
                return std::string();

              auto messages = val.at("messages").as_array();

              auto size =
                  mLastMessage.empty()
                      ? std::min(static_cast<std::size_t>(INIT_LOAD_MESSAGES),
                                 messages.size())
                      : messages.size();

              std::list<pplx::task<std::pair<std::string, std::string>>> tasks;

              for (std::size_t i = 0; i < size; i++) {
                if (messages.at(i).at("id").as_string() == mLastMessage)
                  break;
                tasks.push_back(getMail(messages.at(i).at("id").as_string(),
                                        authHeaderName, authToken, mUserMail));
              }

              std::transform(
                  tasks.begin(), tasks.end(),
                  std::front_inserter(*loadedMessages),
                  [](const pplx::task<std::pair<std::string, std::string>>&
                         task) { return task.get(); });

              auto first = messages.size() > 0
                               ? messages.at(0).at("id").as_string()
                               : "";

              if (tasks.size() == messages.size() && !mLastMessage.empty()) {
                auto pageToken = val.at("nextPageToken").as_string();
                loadMessages(authHeaderName, authToken, pageToken,
                             loadedMessages);
              }

              return first;
            } catch (std::exception& e) {
              std::cout << "catch exception " << e.what()
                        << "when load messages in " << lastPageToken
                        << std::endl;
              throw e;
            }
          });
  return task.get();
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
