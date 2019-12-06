#include "discoveredcontact.h"

DiscoveredContact::DiscoveredContact(
    std::chrono::time_point<std::chrono::system_clock> created,
    std::string channel_moniker,
    std::string contact_adress,
    std::string public_key,
    std::string name,
    std::string dialog_id) :
    m_created(created),
    m_channel_moniker(std::move(channel_moniker)),
    m_contact_adress(std::move(contact_adress)),
    m_public_key(std::move(public_key)), m_name(std::move(name)),
    m_dialog_id(std::move(dialog_id)) {}

DiscoveredContact::DiscoveredContact(std::string channel_moniker,
                                     std::string contact_adress,
                                     std::string public_key,
                                     std::string name,
                                     std::string dialog_id) :
    DiscoveredContact(std::chrono::system_clock::now(),
                      std::move(channel_moniker),
                      std::move(contact_adress),
                      std::move(public_key),
                      std::move(name),
                      std::move(dialog_id)) {}

std::chrono::time_point<std::chrono::system_clock> DiscoveredContact::created()
    const noexcept {
  return m_created;
}

std::string_view DiscoveredContact::channel_moniker() const noexcept {
  return m_channel_moniker;
}

std::string_view DiscoveredContact::contact_adress() const noexcept {
  return m_contact_adress;
}

std::string_view DiscoveredContact::public_key() const noexcept {
  return m_public_key;
}

std::string_view DiscoveredContact::name() const noexcept {
  return m_name;
}

std::string_view DiscoveredContact::dialog_id() const noexcept {
  return m_dialog_id;
}
