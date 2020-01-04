#include "discoveredcontactmarshaller.h"
#include "persistent-storage/utils/store_primitives.h"

#include "primitives/discoveredcontact.h"
#include "store_helpers.h"

void DiscoveredContactMarshaller::restore(DiscoveredContact& elem,
                                          const void* src) {
  std::chrono::time_point<std::chrono::system_clock>::duration duration;
  std::string channel_moniker, contact_adress, public_key, name, dialog_id;

  src = restore_simple_type(duration, src);

  src = prstorage::restore_str(channel_moniker, src);
  src = prstorage::restore_str(contact_adress, src);
  src = prstorage::restore_str(public_key, src);
  src = prstorage::restore_str(name, src);
  src = prstorage::restore_str(dialog_id, src);

  elem = DiscoveredContact(
      std::chrono::time_point<std::chrono::system_clock>(duration),
      channel_moniker, contact_adress, public_key, name, dialog_id);
}

uint32_t DiscoveredContactMarshaller::size(const DiscoveredContact& element) {
  uint32_t size = 0;
  size += sizeof(std::chrono::time_point<std::chrono::system_clock>::duration);
  size += sizeof(std::string::size_type) + element.channel_moniker().length();
  size += sizeof(std::string::size_type) + element.contact_adress().length();
  size += sizeof(std::string::size_type) + element.public_key().length();
  size += sizeof(std::string::size_type) + element.name().length();
  size += sizeof(std::string::size_type) + element.dialog_id().length();
  return size;
}

void DiscoveredContactMarshaller::store(void* dest,
                                        const DiscoveredContact& elem) {
  dest = store_simple_type(elem.created().time_since_epoch(), dest);
  dest = save_str(elem.channel_moniker(), dest);
  dest = save_str(elem.contact_adress(), dest);
  dest = save_str(elem.public_key(), dest);
  dest = save_str(elem.name(), dest);
  dest = save_str(elem.dialog_id(), dest);
}
