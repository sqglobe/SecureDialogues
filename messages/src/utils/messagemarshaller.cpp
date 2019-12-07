#include "messagemarshaller.h"

#include <algorithm>
#include <iostream>
#include <sstream>

const std::string ACTION_MESSAGE = "MESSAGE";
const std::string ACTION_CREATE_DIALOG = "CREATE_DIALOG";
const std::string ACTION_ACCEPT_DIALOG = "ACCEPT_DIALOG";
const std::string ACTION_CLOSE_DIALOG = "CLOSE_DIALOG";
const std::string ACTION_ACK = "ACK";
const std::string ACTION_ABORT = "ABORT";
const std::string ACTION_VERIFY_KEY = "VERIFY_KEY";
const std::string ACTION_KEY_VERIFICATION = "KEY_VERIFICATION";
const std::string ACTION_CONTACT_DISCOVER = "CONTACT_DISCOVER";

const std::string FIELD_ACTION = "ACTION";
const std::string FIELD_DIALOG_ID = "DIALOG";
const std::string FIELD_VERSION = "VERSION";
const std::string FIELD_SEQUENTAL_NUMBER = "SEQUENTAL_NUMBER";
const std::string HEADER = "SECURE DIALOG MESSAGE";
const std::string BODY_DELITIMER =
    "--------------------------------------------------------------";
const std::string FIELD_SIGN = "SIGNATURE";
const std::string FIELD_TIMESTAMP = "TIMESTAMP";

// trim from end (in place)
static inline void rtrim(std::string& s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
                       [](int ch) { return !std::isspace(ch); })
              .base(),
          s.end());
}

struct MessageFields {
  enum State { HEADER, SIGNATURE, BODY };

  DialogMessage::Action mAction;
  std::string mContent;
  std::string mDialogId;
  std::string mAdress;  // adress recive from, or send to
  unsigned long mSeqNumber = 0;
  std::string signature;
  State state;
  long timestamp;
};

//    enum class Action{MESSAGE, CREATE_DIALOG, ACCEPT_DIALOG, CLOSE_DIALOG,
//    CANCEL_DIALOG, ACK, ABORT, SET_KEY, VERIFY_KEY, KEY_VERIFICATION };

class bad_action : public std::invalid_argument {
 public:
  bad_action(const std::string& mess) : std::invalid_argument(mess) {}
};

std::optional<DialogMessage> MessageMarshaller::unmarshall(
    const std::string& message,
    const std::string& address) {
  std::istringstream ss(message);
  std::string line;

  auto lineNumber = 1;

  MessageFields fields;
  fields.state = MessageFields::State::HEADER;

  try {
    while (std::getline(ss, line)) {
      if (!prepareLine(line, lineNumber, fields)) {
        return {};
      }
      lineNumber += 1;
    }
  } catch (std::exception&) {
    return {};
  }

  for (auto it = fields.mContent.rbegin();
       std::isspace(*it) && it != fields.mContent.rend(); ++it) {
    *it = 0x00;
  }
  return std::optional<DialogMessage>(
      std::in_place, fields.mAction, fields.mContent.c_str(), fields.mDialogId,
      address, fields.mSeqNumber, fields.signature, fields.timestamp);
}

std::string MessageMarshaller::marshall(const DialogMessage& message) {
  std::stringstream ss;
  std::string content(message.content());
  for (auto it = content.rbegin(); std::isspace(*it) && it != content.rend();
       ++it) {
    *it = 0x00;
  }

  ss << HEADER << '\n'
     << FIELD_VERSION << ':' << DialogMessage::VERSION << '\n'
     << FIELD_ACTION << ':' << convertAction(message.action()) << '\n'
     << FIELD_SEQUENTAL_NUMBER << ':' << message.sequential() << '\n'
     << FIELD_DIALOG_ID << ':' << message.dialogId() << '\n'
     << FIELD_TIMESTAMP << ':' << message.timestamp() << '\n'
     << FIELD_SIGN << ":" << message.sign() << '\n'
     << BODY_DELITIMER << '\n'
     << content.c_str();
  return ss.str();
}

std::string MessageMarshaller::marshallExceptSignature(
    const DialogMessage& message) {
  std::stringstream ss;
  std::string content(message.content());
  for (auto it = content.rbegin(); std::isspace(*it) && it != content.rend();
       ++it) {
    *it = 0x00;
  }

  ss << HEADER << '\n'
     << FIELD_VERSION << ':' << DialogMessage::VERSION << '\n'
     << FIELD_ACTION << ':' << convertAction(message.action()) << '\n'
     << FIELD_SEQUENTAL_NUMBER << ':' << message.sequential() << '\n'
     << FIELD_DIALOG_ID << ':' << message.dialogId() << '\n'
     << FIELD_TIMESTAMP << ':' << message.timestamp() << '\n'
     << FIELD_SIGN << ":" << '\n'
     << BODY_DELITIMER << '\n'
     << content.c_str();
  return ss.str();
}

bool MessageMarshaller::splitKeyVal(const std::string& source,
                                    std::string& key,
                                    std::string& val,
                                    char ch) {
  auto pos = source.find(ch);
  if (std::string::npos == pos || (source.size() - 1) == pos || 0 == pos) {
    return false;
  }

  key.clear();
  key.append(source, 0, pos);
  val.clear();
  val.append(source, pos + 1, (source.size() - pos - 1));
  return true;
}

DialogMessage::Action MessageMarshaller::convertAction(
    const std::string& action) {
  if (action == ACTION_ACCEPT_DIALOG) {
    return DialogMessage::Action::ACCEPT_DIALOG;
  } else if (action == ACTION_CLOSE_DIALOG) {
    return DialogMessage::Action::CLOSE_DIALOG;
  } else if (action == ACTION_CREATE_DIALOG) {
    return DialogMessage::Action::CREATE_DIALOG;
  } else if (action == ACTION_MESSAGE) {
    return DialogMessage::Action::MESSAGE;
  } else if (action == ACTION_ACK) {
    return DialogMessage::Action::ACK;
  } else if (action == ACTION_ABORT) {
    return DialogMessage::Action::ABORT;
  } else if (action == ACTION_KEY_VERIFICATION) {
    return DialogMessage::Action::KEY_VERIFICATION;
  } else if (action == ACTION_VERIFY_KEY) {
    return DialogMessage::Action::VERIFY_KEY;
  } else if (action == ACTION_CONTACT_DISCOVER) {
    return DialogMessage::Action::CONTACT_DISCOVER;
  } else {
    throw bad_action("Undefined action " + action);
  }
}

std::string MessageMarshaller::convertAction(DialogMessage::Action action) {
  using Action = DialogMessage::Action;
  if (Action::ACCEPT_DIALOG == action) {
    return ACTION_ACCEPT_DIALOG;
  } else if (Action::CLOSE_DIALOG == action) {
    return ACTION_CLOSE_DIALOG;
  } else if (Action::CREATE_DIALOG == action) {
    return ACTION_CREATE_DIALOG;
  } else if (Action::MESSAGE == action) {
    return ACTION_MESSAGE;
  } else if (Action::ACK == action) {
    return ACTION_ACK;
  } else if (Action::ABORT == action) {
    return ACTION_ABORT;
  } else if (Action::KEY_VERIFICATION == action) {
    return ACTION_KEY_VERIFICATION;
  } else if (Action::VERIFY_KEY == action) {
    return ACTION_VERIFY_KEY;
  } else if (Action::CONTACT_DISCOVER == action) {
    return ACTION_CONTACT_DISCOVER;
  }
  throw(std::runtime_error("Invalid action"));
}

bool MessageMarshaller::prepareLine(const std::string& line,
                                    int lineNumber,
                                    MessageFields& fields) {
  std::string key, val;
  if (lineNumber == 1 && line == HEADER) {
    return true;
  } else if (lineNumber == 2 && splitKeyVal(line, key, val) &&
             key == FIELD_VERSION && val == DialogMessage::VERSION) {
    return true;
  } else if (lineNumber == 3 && splitKeyVal(line, key, val) &&
             key == FIELD_ACTION) {
    fields.mAction = convertAction(val);
    return true;
  } else if (lineNumber == 4 && splitKeyVal(line, key, val) &&
             key == FIELD_SEQUENTAL_NUMBER) {
    fields.mSeqNumber = std::stoul(val);
    return true;
  } else if (lineNumber == 5 && splitKeyVal(line, key, val) &&
             key == FIELD_DIALOG_ID) {
    fields.mDialogId = val;
    return true;
  } else if (lineNumber == 6 && splitKeyVal(line, key, val) &&
             key == FIELD_TIMESTAMP) {
    fields.timestamp = std::stol(val);
    return true;
  } else if (lineNumber == 7 && splitKeyVal(line, key, val) &&
             key == FIELD_SIGN) {
    fields.signature = val;
    fields.state = MessageFields::State::SIGNATURE;
    return true;
  } else if (lineNumber >= 8 && line == BODY_DELITIMER) {
    fields.state = MessageFields::State::BODY;
    return true;
  } else if (fields.state == MessageFields::State::SIGNATURE) {
    fields.signature.append("\n").append(line);
    return true;
  } else if (fields.state == MessageFields::State::BODY) {
    fields.mContent.append(line).append("\n");
    return true;
  }
  return false;
}
