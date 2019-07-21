#include "contactmarshaller.h"
#include "persistent-storage/store_primitives.h"

void ContactMarshaller::restore(Contact& elem, const void* src) {
  std::string channelMoniker, name, adress, contactId, publicKey;
  src = prstorage::restore_str(channelMoniker, src);
  src = prstorage::restore_str(name, src);
  src = prstorage::restore_str(adress, src);
  src = prstorage::restore_str(contactId, src);
  src = prstorage::restore_str(publicKey, src);
  elem = {channelMoniker, name, adress, publicKey, contactId};
}

size_t ContactMarshaller::size(const Contact& element) {
  size_t size = 0;
  size += sizeof(std::string::size_type) + element.channelMoniker().length();
  size += sizeof(std::string::size_type) + element.name().length();
  size += sizeof(std::string::size_type) + element.adress().length();
  size += sizeof(std::string::size_type) + element.id().length();
  size += sizeof(std::string::size_type) + element.publicKey().length();
  return size;
}

void ContactMarshaller::store(void* dest, const Contact& elem) {
  dest = prstorage::save_str(elem.channelMoniker(), dest);
  dest = prstorage::save_str(elem.name(), dest);
  dest = prstorage::save_str(elem.adress(), dest);
  dest = prstorage::save_str(elem.id(), dest);
  dest = prstorage::save_str(elem.publicKey(), dest);
}
