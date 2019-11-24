#ifndef COMMUNICATION_HELPERS_H
#define COMMUNICATION_HELPERS_H
#include "containers/storages.h"
#include "primitives/contact.h"
#include "primitives/dialog.h"
#include "primitives/dialogmessage.h"

#include "persistent-storage/wrappers/transparentcontainerelementwrapper.h"

struct MessageSendInfo {
  DialogMessage message;
  std::string_view channel;
};

MessageSendInfo make_text_message(DialogStorage::wrapper_type& dialog,
                                  const Contact& contact,
                                  std::string&& content);
MessageSendInfo make_message(DialogMessage::Action action,
                             DialogStorage::wrapper_type& dialog,
                             const Contact& contact,
                             std::string&& content);

#endif  // COMMUNICATION_HELPERS_H
