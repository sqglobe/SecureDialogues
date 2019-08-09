#include "dialogmarshaller.h"

#include "persistent-storage/utils/store_primitives.h"

#include "store_helpers.h"
/*
 * std::string mDialogId;
  std::string mContactId;
  Status mStatus;
  unsigned long mPeerSequental;
  mutable unsigned long mThisSequental;
 */
void DialogMarshaller::restore(Dialog& elem, const void* src) {
  std::string dialogId, contactId;
  Dialog::Status st;
  unsigned long peerSequental, thisSequental;

  src = prstorage::restore_str(dialogId, src);
  src = prstorage::restore_str(contactId, src);
  src = restore_simple_type(st, src);
  src = restore_simple_type(peerSequental, src);
  src = restore_simple_type(thisSequental, src);

  elem = Dialog(contactId, dialogId, peerSequental, thisSequental, st);
}

size_t DialogMarshaller::size(const Dialog& element) {
  size_t size = 0;
  size += sizeof(std::string::size_type) + element.getDialogId().length();
  size += sizeof(std::string::size_type) + element.getContactId().length();
  size += sizeof(Dialog::Status);
  size += sizeof(element.getPeerSequental());
  size += sizeof(element.getThisSequental());
  return size;
}

void DialogMarshaller::store(void* dest, const Dialog& elem) {
  dest = save_str(elem.getDialogId(), dest);
  dest = save_str(elem.getContactId(), dest);
  dest = store_simple_type(elem.getStatus(), dest);
  dest = store_simple_type(elem.getPeerSequental(), dest);
  dest = store_simple_type(elem.getThisSequental(), dest);
}
