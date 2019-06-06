#ifndef OAUTHADAPTER_H
#define OAUTHADAPTER_H

#include <list>
#include "interfaces/abstractchanneladapter.h"

#include "oauth-agents/interfaces/apiagent.h"
#include "oauth-agents/interfaces/oauthagent.h"

class OauthAdapter : public AbstractChannelAdapter {
 public:
  using AbstractChannelAdapter::AbstractChannelAdapter;

 public:
  void send(const std::string& message,
            const std::string& adress) noexcept(false) override;
  std::pair<std::string, std::string> receive() noexcept(false) override;
  bool connect() override;

 private:
  std::unique_ptr<OAuthAgent> mOauthAgent;
  std::unique_ptr<ApiAgent> mApiAgent;
  std::list<std::pair<std::string, std::string> > mMessages;
};

#endif  // OAUTHADAPTER_H
