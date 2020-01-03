#include "communication_helpers.h"
#include <libintl.h>
#include "fmt/core.h"
#include "fmt/format.h"

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
    throw invalid_dialog_action(fmt::format(
        dgettext("messages",
                 "Message with action {} cant be made for dialog state {}"),
        static_cast<int>(action), static_cast<int>(dialog->getStatus())));
  }
  return {DialogMessage(
              action, std::move(content), std::string(dialog->getDialogId()),
              std::string(contact.adress()), dialog->makeNextSequental()),
          contact.channelMoniker()};
}
