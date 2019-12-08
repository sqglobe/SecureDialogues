#ifndef DESPATCHERRORSINKFAKE_H
#define DESPATCHERRORSINKFAKE_H

#include "interfaces/abstractdespatcherrorssink.h"
#include "primitives/dialogmessage.h"

class DespatchErrorSinkFake : public AbstractDespatchErrorsSink {
 public:
  virtual void error(
      DespatchError error,
      const DialogMessage& message,
      const std::string& channel_moniker) noexcept(false) override;

 public:
  DespatchError happened_error;
  DialogMessage message{DialogMessage::Action::ACK, "", ""};
  std::string channel_moniker;
};

void DespatchErrorSinkFake::error(DespatchError error,
                                  const DialogMessage& message,
                                  const std::string& channel_moniker) {
  happened_error = error;
  this->message = message;
  this->channel_moniker = channel_moniker;
}
#endif  // DESPATCHERRORSINKFAKE_H
