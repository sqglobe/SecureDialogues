#include "discovercontactagent.h"
#include "interfaces/abstractmessagedespatcher.h"
#include "interfaces/cryptosystem.h"
#include "primitives/dialogmessage.h"
#include "utils.h"

#include <random>

const std::string ContentDelitimer = "\n------------------------------\n";

static unsigned long make_sequental() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<unsigned long> dis(
      0, std::numeric_limits<unsigned int>::max());
  return dis(gen);
}

DiscoverContactAgent::DiscoverContactAgent(
    std::shared_ptr<CryptoSystem> crypto,
    std::shared_ptr<AbstractMessageDespatcher> despatcher) :
    m_crypto(std::move(crypto)),
    m_despatcher(std::move(despatcher)) {}

std::pair<std::string, std::string> DiscoverContactAgent::getNameAndPublickKey(
    const DialogMessage& message) noexcept(false) {
  auto pos = message.content().find_first_of(ContentDelitimer);
  if (pos == std::string_view::npos) {
    throw std::runtime_error("Cant find delitimer");
  }

  std::string name(message.content().substr(0, pos));
  std::string key(message.content().substr(pos + ContentDelitimer.length()));
  return std::make_pair(std::move(name), std::move(key));
}

void DiscoverContactAgent::discover(std::string name,
                                    std::string address,
                                    std::string channel_moniker) {
  std::string content{name};
  content.append(ContentDelitimer).append(m_crypto->exportPublicKey());

  DialogMessage message{DialogMessage::Action::CONTACT_DISCOVER,
                        std::move(content), make_uiid(), std::move(address),
                        make_sequental()};

  m_despatcher->sendAndForget(std::move(message), channel_moniker);
}
