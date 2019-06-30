#include "contactmarshaller.h"

#include "store_primitives.h"

void ContactMarshaller::restore(std::shared_ptr<Contact>& elem,
                                const void* src) {
  std::string channelMoniker, name, adress, contactId, publicKey;
  src = restore_str(channelMoniker, src);
  src = restore_str(name, src);
  src = restore_str(adress, src);
  src = restore_str(contactId, src);
  src = restore_str(publicKey, src);
  elem = std::make_shared<Contact>(channelMoniker, name, adress, publicKey,
                                   contactId);
}

size_t ContactMarshaller::size(const std::shared_ptr<Contact>& element) {
  size_t size = 0;
  size += sizeof(std::string::size_type) + element->channelMoniker().length();
  size += sizeof(std::string::size_type) + element->name().length();
  size += sizeof(std::string::size_type) + element->adress().length();
  size += sizeof(std::string::size_type) + element->id().length();
  size += sizeof(std::string::size_type) + element->publicKey().length();
  return size;
}

void ContactMarshaller::store(void* dest,
                              const std::shared_ptr<Contact>& elem) {
  dest = save_str(elem->channelMoniker(), dest);
  dest = save_str(elem->name(), dest);
  dest = save_str(elem->adress(), dest);
  dest = save_str(elem->id(), dest);
  dest = save_str(elem->publicKey(), dest);
}
