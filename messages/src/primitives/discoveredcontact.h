#ifndef DISCOVEREDCONTACT_H
#define DISCOVEREDCONTACT_H

#include <chrono>
#include <string>
#include <string_view>

class DiscoveredContact {
 public:
  DiscoveredContact() = default;
  DiscoveredContact(std::chrono::time_point<std::chrono::system_clock> created,
                    std::string channel_moniker,
                    std::string contact_adress,
                    std::string public_key,
                    std::string name,
                    std::string dialog_id);
  DiscoveredContact(std::string channel_moniker,
                    std::string contact_adress,
                    std::string public_key,
                    std::string name,
                    std::string dialog_id);

 public:
  std::chrono::time_point<std::chrono::system_clock> created() const noexcept;
  std::string_view channel_moniker() const noexcept;
  std::string_view contact_adress() const noexcept;
  std::string_view public_key() const noexcept;
  std::string_view name() const noexcept;
  std::string_view dialog_id() const noexcept;

 private:
  std::chrono::time_point<std::chrono::system_clock> m_created;
  std::string m_channel_moniker;
  std::string m_contact_adress;
  std::string m_public_key;
  std::string m_name;
  std::string m_dialog_id;
};

#endif  // DISCOVEREDCONTACT_H
