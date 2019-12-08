#include "despatcherrorssink.h"

#include "communication/discovercontactagent.h"
#include "primitives/dialogmessage.h"

#include "spdlog/spdlog.h"

#include <spdlog/sinks/stdout_color_sinks.h>

DespatchErrorsSink::DespatchErrorsSink(
    std::shared_ptr<DiscoveredContactStorage> discoveredStorage) :
    m_DiscoveredStorage(std::move(discoveredStorage)) {}

void DespatchErrorsSink::error(DespatchError error,
                               const DialogMessage& message,
                               const std::string& channel_moniker) {
  using E = DespatchError;
  if (error == E::ContactNotFound) {
    if (message.action() == DialogMessage::Action::CONTACT_DISCOVER) {
      auto const& [name, publicKey] =
          DiscoverContactAgent::getNameAndPublickKey(message);
      m_DiscoveredStorage->add({channel_moniker, std::string(message.adress()),
                                publicKey, name,
                                std::string(message.dialogId())});
    } else {
      spdlog::get("root_logger")
          ->error("Не найден контакт {0} ", message.adress());
    }
  } else if (error == E::MessageExpired) {
    spdlog::get("root_logger")
        ->error("Get old message {0} for address {1} channel {2}",
                message.content(), message.adress(), channel_moniker);
  } else if (error == E::ExceptionThrown) {
    spdlog::get("root_logger")
        ->error("При обработки сообщения  от {0} произошла ошибка",
                message.adress());
  } else if (error == E::SignatureBroken) {
    spdlog::get("root_logger")
        ->error(
            "Подпись для адреса {0} не верна. Возможно публичный ключ "
            "изменился "
            "или это является следствием атаки",
            message.adress());
  }
}
