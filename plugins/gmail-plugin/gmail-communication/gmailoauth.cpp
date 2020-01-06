#include "gmailoauth.h"

#include <nlohmann/json.hpp>
#include "uribuilder.h"

GmailOauth::GmailOauth(std::string clientId,
                       std::string clientSecret,
                       std::string redirectUri) :
    mClientId(std::move(clientId)),
    mClientSecret(std::move(clientSecret)),
    mRedirectUri(std::move(redirectUri)) {}

GmailOauth::GmailOauth(std::string clientId,
                       std::string clientSecret,
                       std::string redirectUri,
                       std::string refreshToken) :
    mClientId(std::move(clientId)),
    mClientSecret(std::move(clientSecret)),
    mRedirectUri(std::move(redirectUri)),
    mRefreshToken(std::move(refreshToken)) {}

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
  auto [code, res] = mRequest.post(
      "https://www.googleapis.com/oauth2/v4/token", builder.getQuery(),
      {{"Content-Type", "application/x-www-form-urlencoded"}});

  if (HttpCode::OK != code) {
    throw std::runtime_error("Failed to send message");
  }

  auto body = nlohmann::json::parse(res);
  this->mAccessToken = body.at("access_token");
  this->mRefreshToken = body.at("refresh_token");
  this->mTokenType = body.at("token_type");
  this->mExpiresAt = std::chrono::system_clock::now() +
                     std::chrono::seconds(body.at("expires_in"));
}

void GmailOauth::refreshAccessToken() {
  [[maybe_unused]] std::lock_guard<std::mutex> guard(mMutex);
  UriBuilder builder;
  builder.appendQuery("refresh_token", mRefreshToken)
      .appendQuery("client_id", mClientId)
      .appendQuery("client_secret", mClientSecret)
      .appendQuery("grant_type", "refresh_token");
  auto [code, response] = mRequest.post(
      "https://www.googleapis.com/oauth2/v4/token", builder.getQuery(),
      {{"Content-Type", "application/x-www-form-urlencoded"}});

  if (HttpCode::OK != code) {
    throw std::runtime_error("Failed to send message");
  }

  auto body = nlohmann::json::parse(response);
  mAccessToken = body.at("access_token");
  mExpiresAt = std::chrono::system_clock::now() +
               std::chrono::seconds(body.at("expires_in"));
  mTokenType = body.at("token_type");
}

std::pair<std::string, std::string> GmailOauth::getAuthParam() const {
  [[maybe_unused]] std::lock_guard<std::mutex> guard(mMutex);
  return std::make_pair<std::string, std::string>(
      "Authorization", mTokenType + " " + mAccessToken);
}

bool GmailOauth::isExpired() const {
  [[maybe_unused]] std::lock_guard<std::mutex> guard(mMutex);
  return std::chrono::system_clock::now() >= mExpiresAt;
}

std::string GmailOauth::getRefreshToken() const {
  [[maybe_unused]] std::lock_guard<std::mutex> guard(mMutex);
  return mRefreshToken;
}
