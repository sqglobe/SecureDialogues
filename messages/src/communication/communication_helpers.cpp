#include "communication_helpers.h"

MessageSendInfo make_text_message(DialogStorage::wrapper_type& dialog,
                                  const Contact& contact,
                                  std::string&& content) {
  return make_message(DialogMessage::Action::MESSAGE, dialog, contact,
                      std::move(content));
}

MessageSendInfo make_message(DialogMessage::Action action,
                             DialogStorage::wrapper_type& dialog,
                             const Contact& contact,
                             std::string&& content) {
  if (!dialog->isMessageActionAllowed(action)) {
    std::stringstream ss;
    ss << "Message with action " << static_cast<int>(action)
       << " cant be made for dialog state "
       << static_cast<int>(dialog->getStatus());
    throw invalid_dialog_action(ss.str());
  }
  return {DialogMessage(
              action, std::move(content), std::string(dialog->getDialogId()),
              std::string(contact.adress()), dialog->makeNextSequental()),
          contact.channelMoniker()};
}
