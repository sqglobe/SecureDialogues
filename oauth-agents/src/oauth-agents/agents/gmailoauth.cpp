#include "gmailoauth.h"

#include <cpprest/http_client.h>

#include "oauth-agents/exceptions/httpfailexception.h"

GmailOauth::GmailOauth(const std::string& clientId,
                       const std::string& clientSecret,
                       const std::string& redirectUri) :
    mClientId(clientId),
    mClientSecret(clientSecret), mRedirectUri(redirectUri) {}

GmailOauth::GmailOauth(const std::string& clientId,
                       const std::string& clientSecret,
                       const std::string& redirectUri,
                       const std::string& refreshToken) :
    GmailOauth(clientId, clientSecret, redirectUri) {
  mRefreshToken = refreshToken;
}

std::string GmailOauth::getUserUrl() const {
  using namespace web;

  uri_builder builder("https://accounts.google.com/o/oauth2/v2/auth");
  builder.append_query("client_id", mClientId)
      .append_query("redirect_uri", mRedirectUri)
      .append_query("scope",
                    "https://www.googleapis.com/auth/gmail.send "
                    "https://www.googleapis.com/auth/gmail.readonly")
      .append_query("response_type", "code")
      .append_query("access_type", "offline");
  return builder.to_string();
}

void GmailOauth::loadAccessToken(const std::string& userCode) {
  using namespace web::http::client;  // HTTP client features
  http_client client(U("https://www.googleapis.com/oauth2/v4/token"));
  web::uri_builder builder;
  builder.append_query("code", userCode)
      .append_query("client_id", mClientId)
      .append_query("client_secret", mClientSecret)
      .append_query("redirect_uri", mRedirectUri)
      .append_query("grant_type", "authorization_code");

  auto task = client
                  .request(web::http::methods::POST, "", builder.query(),
                           "application/x-www-form-urlencoded")
                  .then([](web::http::http_response response) {
                    if (web::http::status_codes::OK != response.status_code()) {
                      throw HttpFailException(
                          response.status_code(),
                          "https://www.googleapis.com/oauth2/v4/token",
                          response.extract_string().get());
                    }
                    return response.extract_json();
                  })
                  .then([this](web::json::value val) {
                    mAccessToken = val.at("access_token").as_string();
                    mRefreshToken = val.at("refresh_token").as_string();
                    mTokenType = val.at("token_type").as_string();
                    mExpiresAt =
                        std::chrono::system_clock::now() +
                        std::chrono::seconds(val.at("expires_in").as_integer());
                  });
  task.wait();
}

void GmailOauth::refreshAccessToken() {
  using namespace web;
  using namespace web::http::client;  // HTTP client features

  http_client client("https://www.googleapis.com/oauth2/v4/token");
  uri_builder builder;
  builder.append_query("refresh_token", mRefreshToken)
      .append_query("client_id", mClientId)
      .append_query("client_secret", mClientSecret)
      .append_query("grant_type", "refresh_token");
  auto task = client
                  .request(web::http::methods::POST, "", builder.query(),
                           "application/x-www-form-urlencoded")
                  .then([&builder](web::http::http_response response) {
                    if (web::http::status_codes::OK != response.status_code()) {
                      throw HttpFailException(
                          response.status_code(),
                          "https://www.googleapis.com/oauth2/v4/token  " +
                              builder.query(),
                          response.to_string());
                    }
                    return response.extract_json();
                  })
                  .then([this](web::json::value val) {
                    mAccessToken = val.at("access_token").as_string();
                    mExpiresAt =
                        std::chrono::system_clock::now() +
                        std::chrono::seconds(val.at("expires_in").as_integer());
                    mTokenType = val.at("token_type").as_string();
                  });
  task.wait();
}

std::pair<std::string, std::string> GmailOauth::getAuthParam() const {
  return std::make_pair<std::string, std::string>(
      "Authorization", mTokenType + " " + mAccessToken);
}

bool GmailOauth::isExpired() const {
  return std::chrono::system_clock::now() >= mExpiresAt;
}

std::string GmailOauth::getRefreshToken() const {
  return mRefreshToken;
}
