#include "vkoauth.h"

#include "uribuilder.h"

VkOauth::VkOauth(std::string clientId, std::string redirectUri) :
    mClientId(std::move(clientId)), mRedirectUri(std::move(redirectUri)) {}

VkOauth::VkOauth(std::string clientId,
                 std::string redirectUri,
                 std::string accessToken) :
    mClientId(std::move(clientId)),
    mRedirectUri(std::move(redirectUri)), mAccessToken(std::move(accessToken)) {
}

std::string VkOauth::getUserUrl() const {
  UriBuilder builder("https://oauth.vk.com/authorize");

  builder.appendQuery("client_id", mClientId)
      .appendQuery("redirect_uri", mRedirectUri)
      .appendQuery("scope", "69632")
      .appendQuery("display", "page")
      .appendQuery("response_type", "token")
      .appendQuery("v", "5.92");
  return builder.getUri();
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
