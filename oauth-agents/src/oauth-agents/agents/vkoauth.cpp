#include "vkoauth.h"
#include <cpprest/http_client.h>

VkOauth::VkOauth(const std::string& clientId, const std::string& redirectUri) :
    mClientId(clientId), mRedirectUri(redirectUri) {}

VkOauth::VkOauth(const std::string& clientId,
                 const std::string& redirectUri,
                 const std::string& accessToken) :
    VkOauth(clientId, redirectUri) {
  mAccessToken = accessToken;
}

std::string VkOauth::getUserUrl() const {
  using namespace web;

  uri_builder builder("https://oauth.vk.com/authorize");
  builder.append_query("client_id", mClientId)
      .append_query("redirect_uri", mRedirectUri)
      .append_query("scope", "69632")
      .append_query("display", "page")
      .append_query("response_type", "token")
      .append_query("v", "5.92");
  return builder.to_string();
}

void VkOauth::loadAccessToken(const std::string& userCode) {
  mAccessToken = userCode;
}

void VkOauth::refreshAccessToken() {}

std::pair<std::string, std::string> VkOauth::getAuthParam() const {
  return std::make_pair<std::string, std::string>("access_token",
                                                  std::string(mAccessToken));
}

bool VkOauth::isExpired() const {
  return false;
}

std::string VkOauth::getRefreshToken() const {
  return mAccessToken;
}
