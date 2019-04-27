#include "gmailoauth.h"

#include <nlohmann/json.hpp>
#include "httprequest.h"
#include "uribuilder.h"

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
  UriBuilder builder("https://accounts.google.com/o/oauth2/v2/auth");
  builder.appendQuery("client_id", mClientId)
      .appendQuery("redirect_uri", mRedirectUri)
      .appendQuery("scope",
                   "https://www.googleapis.com/auth/gmail.send "
                   "https://www.googleapis.com/auth/gmail.readonly")
      .appendQuery("response_type", "code")
      .appendQuery("access_type", "offline");
  return builder.getUri();
}

void GmailOauth::loadAccessToken(const std::string& userCode) {
  UriBuilder builder;
  builder.appendQuery("code", userCode)
      .appendQuery("client_id", mClientId)
      .appendQuery("client_secret", mClientSecret)
      .appendQuery("redirect_uri", mRedirectUri)
      .appendQuery("grant_type", "authorization_code");
  HttpRequest request;
  auto res = request.post(
      "https://www.googleapis.com/oauth2/v4/token", builder.getQuery(),
      {{"Content-Type", "application/x-www-form-urlencoded"}});
  auto body = nlohmann::json::parse(res);
  this->mAccessToken = body.at("access_token");
  this->mRefreshToken = body.at("refresh_token");
  this->mTokenType = body.at("token_type");
  this->mExpiresAt = std::chrono::system_clock::now() +
                     std::chrono::seconds(body.at("expires_in"));
}

void GmailOauth::refreshAccessToken() {
  UriBuilder builder;
  builder.appendQuery("refresh_token", mRefreshToken)
      .appendQuery("client_id", mClientId)
      .appendQuery("client_secret", mClientSecret)
      .appendQuery("grant_type", "refresh_token");
  HttpRequest request;
  auto response = request.post(
      "https://www.googleapis.com/oauth2/v4/token", builder.getQuery(),
      {{"Content-Type", "application/x-www-form-urlencoded"}});
  auto body = nlohmann::json::parse(response);
  mAccessToken = body.at("access_token");
  mExpiresAt = std::chrono::system_clock::now() +
               std::chrono::seconds(body.at("expires_in"));
  mTokenType = body.at("token_type");
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
