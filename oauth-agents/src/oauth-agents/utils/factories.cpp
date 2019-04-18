#include "factories.h"

#include <assert.h>
#include "oauth-agents/agents/gmailapi.h"
#include "oauth-agents/agents/gmailoauth.h"
#include "oauth-agents/agents/vkapi.h"
#include "oauth-agents/agents/vkoauth.h"

const std::string REDIRECT_URL = "urn:ietf:wg:oauth:2.0:oob";

const std::string VK_REDIRECT_URI = "https://oauth.vk.com/blank.html";

class GmailOauthFactory : public OauthFactory {
 public:
  virtual std::unique_ptr<ApiAgent> makeApiAgent(
      const std::string& clientId) const override {
    return std::make_unique<GmailApi>(clientId);
  }
  virtual std::unique_ptr<OAuthAgent> makeOauthAgent() const override {
    return std::make_unique<GmailOauth>(GOOGLE_CLIENT_ID, GOOGLE_CLIENT_SECRET,
                                        REDIRECT_URL);
  }
  virtual std::unique_ptr<OAuthAgent> makeOauthAgent(
      const std::string& refreshToken) const override {
    return std::make_unique<GmailOauth>(GOOGLE_CLIENT_ID, GOOGLE_CLIENT_SECRET,
                                        REDIRECT_URL, refreshToken);
  }
};

class VkOauthFactory : public OauthFactory {
 public:
  virtual std::unique_ptr<ApiAgent> makeApiAgent(
      const std::string& clientId) const override {
    return std::make_unique<VkApi>(clientId);
  }
  virtual std::unique_ptr<OAuthAgent> makeOauthAgent() const override {
    return std::make_unique<VkOauth>(VK_CLIENT_ID, VK_REDIRECT_URI);
  }
  virtual std::unique_ptr<OAuthAgent> makeOauthAgent(
      const std::string& refreshToken) const override {
    return std::make_unique<VkOauth>(VK_CLIENT_ID, VK_REDIRECT_URI,
                                     refreshToken);
  }
};

std::unique_ptr<OauthFactory> makeFactory(AgentType type) {
  switch (type) {
    case AgentType::GMAIL:
      return std::make_unique<GmailOauthFactory>();
    case AgentType::VK:
      return std::make_unique<VkOauthFactory>();
  }
}
