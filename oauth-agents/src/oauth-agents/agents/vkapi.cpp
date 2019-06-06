#include "vkapi.h"

#include <nlohmann/json.hpp>
#include <random>
#include "oauth-agents/utils/base64.h"
#include "uribuilder.h"

#include <nlohmann/json.hpp>
#include "uribuilder.h"

#include <limits>
#include <tuple>

#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/spdlog.h"

#include "oauth-agents/exceptions/oauth-exceptions.h"

#include "httpcode.h"

// https://vk.com/dev/using_longpoll

static std::shared_ptr<spdlog::logger> LOGGER =
    spdlog::stdout_color_mt("vk-api-logger");

enum MESSAGE_FLAGS { OUTBOX = 2, CHAT = 16 };

enum MESSAGE_ELEMETS {
  TYPE = 0,
  FLAGS = 2,
  PEER = 3,
  TEXT = 5,
  ELEMENTS_COUNT
};

std::string getRaindomId() {
  auto seed1 = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 generator(seed1);
  std::uniform_int_distribution<unsigned int> distribution(
      1, std::numeric_limits<unsigned int>::max());
  return std::to_string(
      distribution(generator));  // generates number in the range 1..6
}

std::string getErrorDescription(int error) {
  switch (error) {
    case 15:
      return "Access denied";
    case 900:
      return "Нельзя отправлять сообщение пользователю из черного списка";
    case 902:
      return "Нельзя отправлять сообщения этому пользователю в связи с "
             "настройками приватности";
    case 914:
      return "Сообщение слишком длинное";
    case 936:
      return "Контакт не найден";

    case 2:
      return "Приложение выключено";
    case 5:
      return "Авторизация пользователя не удалась";
    case 6:
      return "Слишком много запросов в секунду";
    case 7:
      return "Нет прав для выполнения этого действия";
  }
  return "Ошибка неизвестна " + std::to_string(error);
}

VkApi::VkApi(std::string userId) : mUserId(std::move(userId)) {}

std::list<std::pair<std::string, std::string> > VkApi::getMessages(
    const std::string& authHeaderName,
    const std::string& authToken) {
  if (mServer.empty()) {
    std::tie(mServer, mKey, mLastNumber) =
        getLongPollServer(authHeaderName, authToken);
  }

  UriBuilder builder("https://" + mServer);
  builder.appendQuery("act", "a_check")
      .appendQuery("key", mKey)
      .appendQuery("ts", std::to_string(mLastNumber))
      .appendQuery("wait", "25")
      .appendQuery("version", "3");
  auto [resp_code, response] = mRequest.get(builder.getUri());

  if (resp_code != HttpCode::OK) {
    throw HttpError(resp_code);
  }

  auto body = nlohmann::json::parse(response);
  if (body.contains("failed")) {
    if (auto failed = body.at("failed"); failed == 1) {
      mLastNumber = body.at("ts");
    } else {
      std::tie(mServer, mKey, mLastNumber) =
          getLongPollServer(authHeaderName, authToken);
    }
    return std::list<std::pair<std::string, std::string> >();
  }

  mLastNumber = body.at("ts");
  auto updates = body.at("updates");
  std::list<std::pair<std::string, std::string> > messages;
  for (auto element : updates) {
    if (element.size() >= ELEMENTS_COUNT && element.at(TYPE) == 4) {
      auto [flags, peer, text] =
          std::tuple(static_cast<int>(element.at(FLAGS)),
                     static_cast<int>(element.at(PEER)), element.at(TEXT));
      if (!(flags & OUTBOX)) {
        messages.emplace_back(std::to_string(peer), text);
      }
    }
  }
  return messages;
}

void VkApi::sendMessage(const std::string& to,
                        const std::string& body,
                        const std::string& authHeaderName,
                        const std::string& authToken) {
  UriBuilder builder;
  builder.appendQuery("v", "5.92")
      .appendQuery("user_id", to)
      .appendQuery("peer_id", to)
      .appendQuery("message", body)
      .appendQuery(authHeaderName, authToken)
      .appendQuery("random_id", getRaindomId());

  auto [resp_code, response] = mRequest.post(
      "https://api.vk.com/method/messages.send", builder.getQuery(),
      {{"Content-type", "application/x-www-form-urlencoded"}});

  if (resp_code != HttpCode::OK) {
    throw HttpError(resp_code);
  }

  auto json = nlohmann::json::parse(response);
  if (json.contains("error")) {
    std::string errorMessage = "Не удалось отправить сообщение ";
    errorMessage.append(to)
        .append(". Причина: ")
        .append(getErrorDescription(json.at("error").at("error_code")));
    if (json.at("error").at("error_code") == 5) {
      throw AuthFailException(errorMessage);
    }
    throw ApiCallError(errorMessage);
  }
}

std::tuple<std::string, std::string, int> VkApi::getLongPollServer(
    const std::string& authHeaderName,
    const std::string& authToken) {
  UriBuilder builder("https://api.vk.com/method/");
  builder.appendPath("messages.getLongPollServer")
      .appendQuery("v", "5.92")
      .appendQuery(authHeaderName, authToken)
      .appendQuery("lp_version", "3");
  auto [resp_code, response] = mRequest.get(builder.getUri());
  if (resp_code != HttpCode::OK) {
    throw HttpError(resp_code);
  }
  auto json = nlohmann::json::parse(response);
  auto data = json.at("response");

  if (json.contains("error")) {
    std::string errorMessage = "Ошибка взаимодействия с VK. ";
    errorMessage.append("Причина: ")
        .append(getErrorDescription(json.at("error").at("error_code")));
    if (json.at("error").at("error_code") == 5) {
      throw AuthFailException(errorMessage);
    }
    throw ApiCallError(errorMessage);
  }

  return std::make_tuple(data.at("server"), data.at("key"), data.at("ts"));
}
