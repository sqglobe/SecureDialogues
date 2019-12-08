#ifndef ABSTRACTDESPATCHERRORSSINK_H
#define ABSTRACTDESPATCHERRORSSINK_H

#include <string>

class DialogMessage;

enum class DespatchError {
  ContactNotFound,
  MessageExpired,
  SignatureBroken,
  ExceptionThrown
};

class AbstractDespatchErrorsSink {
 public:
  virtual ~AbstractDespatchErrorsSink() = default;

 public:
  virtual void error(DespatchError error,
                     const DialogMessage& message,
                     const std::string& channel_moniker) noexcept(false) = 0;
};

#endif  // ABSTRACTDESPATCHERRORSSINK_H
