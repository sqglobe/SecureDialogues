#ifndef DISCOVERCONTACTAGENT_H
#define DISCOVERCONTACTAGENT_H

#include <memory>

class AbstractMessageDespatcher;
class CryptoSystem;
class DialogMessage;

class DiscoverContactAgent {
 public:
  DiscoverContactAgent(std::shared_ptr<CryptoSystem> crypto,
                       std::shared_ptr<AbstractMessageDespatcher> despatcher);

 public:
  static std::pair<std::string, std::string> getNameAndPublickKey(
      const DialogMessage& message) noexcept(false);

 public:
  void discover(std::string name,
                std::string address,
                std::string channel_moniker);

 private:
  std::shared_ptr<CryptoSystem> m_crypto;
  std::shared_ptr<AbstractMessageDespatcher> m_despatcher;
};

#endif  // DISCOVERCONTACTAGENT_H
