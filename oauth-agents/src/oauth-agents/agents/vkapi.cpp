#include "vkapi.h"

#include <cpprest/http_client.h>
#include "oauth-agents/exceptions/httpfailexception.h"
#include "oauth-agents/utils/base64.h"

#include <tuple>
#include <limits>

#include "spdlog/spdlog.h"
#include <spdlog/sinks/stdout_color_sinks.h>

//https://vk.com/dev/using_longpoll

static std::shared_ptr<spdlog::logger>LOGGER =
    spdlog::stdout_color_mt("vk-api-logger");

enum MESSAGE_FLAGS {OUTBOX = 2, CHAT = 16};

enum MESSAGE_ELEMETS { TYPE = 0, FLAGS = 2, PEER = 3, TEXT = 5, ELEMENTS_COUNT };

std::string getRaindomId(){
    auto seed1 = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator (seed1);
    std::uniform_int_distribution<unsigned int> distribution(1, std::numeric_limits<unsigned int>::max());
    return std::to_string(distribution(generator));  // generates number in the range 1..6
}

std::string getErrorDescription(int error){
    switch(error){
      case 15:  return "Access denied";
      case 900: return "Нельзя отправлять сообщение пользователю из черного списка";
      case 902: return "Нельзя отправлять сообщения этому пользователю в связи с настройками приватности";
      case 914: return "Сообщение слишком длинное";
      case 936: return "Контакт не найден";

      case 2: return "Приложение выключено";
      case 5: return "Авторизация пользователя не удалась";
      case 6: return "Слишком много запросов в секунду";
      case 7: return "Нет прав для выполнения этого действия";
    }
    return "Ошибка неизвестна " + std::to_string(error);
}

VkApi::VkApi(const std::string &userId): mUserId(userId)
{

}

std::list<std::pair<std::string, std::string> > VkApi::getMessages(const std::string &authHeaderName, const std::string &authToken)
{
    using namespace web::http::client;  // HTTP client features
    if(mServer.empty()){
        std::tie(mServer, mKey, mLastNumber) = getLongPollServer(authHeaderName, authToken);
    }
    http_client client(U("https://" + mServer));
    web::uri_builder builder;
    builder.append_query("act", "a_check")
           .append_query("key", mKey)
           .append_query("ts", std::to_string( mLastNumber) )
           .append_query("wait", "25")
           .append_query("version", "3");
    web::http::http_request request(web::http::methods::GET);
    request.set_request_uri(builder.to_string());
    auto task = client.request(request)
         .then([&builder](web::http::http_response response){
            if (web::http::status_codes::OK != response.status_code()) {
              throw HttpFailException(
                  response.status_code(),
                  "https://api.vk.com/method" +
                      builder.to_string(),
                  response.extract_string().get());
            }
            return response.extract_json();
         })
        .then([this, &authHeaderName, &authToken](web::json::value val){
            if(val.has_field("failed")){
               auto failed = val.at("failed").as_integer();
               if(failed == 1){
                   mLastNumber = val.at("ts").as_integer();
               }else{
                   std::tie(mServer, mKey, mLastNumber) = getLongPollServer(authHeaderName, authToken);
               }
              return std::list<std::pair<std::string, std::string> >();
            }
            mLastNumber = val.as_object().at("ts").as_integer();
            auto updates = val.at("updates").as_array();
            std::list<std::pair<std::string, std::string> > messages;
            LOGGER->debug("Recieved values: {0}", val.serialize());
            for(auto up: updates){
                LOGGER->debug("update element: {0}", up.serialize());
                if(auto element = up.as_array(); element.size() >= ELEMENTS_COUNT && element.at(TYPE).as_integer() == 4){
                    auto [flags, peer, text] = std::tuple(element.at(FLAGS).as_integer(), element.at(PEER).as_integer(), element.at(TEXT).as_string());
                    if(!(flags & OUTBOX)) {
                       messages.push_back(std::pair(std::to_string(peer), text));
                    }
                }
            }
            return messages;
        });
    return task.get();
}

void VkApi::sendMessage(const std::string &to, const std::string &body, const std::string &authHeaderName, const std::string &authToken)
{
     using namespace web::http::client;  // HTTP client features
     http_client client(U("https://api.vk.com/method/messages.send"));
     web::uri_builder builder;
     builder.append_query("v", "5.92")
            .append_query("user_id", to)
            .append_query("peer_id", to)
            .append_query("message", body)
            .append_query(authHeaderName, authToken)
            .append_query("random_id", getRaindomId());
     web::http::http_request request(web::http::methods::POST);
     request.headers().add("Content-type", "application/x-www-form-urlencoded");
     request.set_body(builder.query());
     LOGGER->debug("Send message to vk: {0}", builder.query());
     client.request(request).then([](web::http::http_response response){
         if (web::http::status_codes::OK != response.status_code()) {
           throw HttpFailException(
               response.status_code(),
               "https://api.vk.com/method/messages.send",
               response.extract_string().get());
         }
         return response.extract_json();
     }).then([&to](web::json::value val){
         if(val.has_field("error")){
             std::string errorMessage = "Не удалось отправить сообщение ";
             errorMessage.append(to).append(". Причина: ").append(getErrorDescription(val.at("error").at("error_code").as_integer()));
             throw std::runtime_error(errorMessage);
         }
     }).wait();
}

std::tuple<std::string, std::string, int> VkApi::getLongPollServer(const std::string &authHeaderName, const std::string &authToken)
{
    using namespace web::http::client;  // HTTP client features
    web::uri_builder builder;
    builder.append_path("messages.getLongPollServer")
           .append_query("v", "5.92")
           .append_query(authHeaderName, authToken)
           .append_query("lp_version", "3");

    http_client client(U("https://api.vk.com/method/"));
    web::http::http_request request(web::http::methods::GET);
    request.set_request_uri(builder.to_string());
    auto task = client.request(request)
        .then([&builder](web::http::http_response response) {
          if (web::http::status_codes::OK != response.status_code()) {
            throw HttpFailException(
                response.status_code(),
                "https://api.vk.com/method" +
                    builder.to_string(),
                response.extract_string().get());
          }
          return response.extract_json();
        })
        .then([](web::json::value val) {
            auto data = val.at("response");
            return std::make_tuple(data.at("server").as_string(), data.at("key").as_string(), data.at("ts").as_integer());
        });
    return task.get();
}
