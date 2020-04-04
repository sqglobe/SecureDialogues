#ifndef RECIEVEDMESSAGESITERATOR_H
#define RECIEVEDMESSAGESITERATOR_H

#include <QtGlobal>
#include "pluginapierrorcodes.h"

class Q_DECL_EXPORT RecievedMessagesIterator {
 public:
  virtual ~RecievedMessagesIterator() = default;

 public:
  [[nodiscard]] virtual bool hasNext() const noexcept = 0;
  virtual bool next() noexcept = 0;

  [[nodiscard]] virtual PluginApiErrorCodes error() const noexcept = 0;

  [[nodiscard]] virtual const char* address() const noexcept = 0;
  [[nodiscard]] virtual const char* message() const noexcept = 0;
};

#endif  // RECIEVEDMESSAGESITERATOR_H
