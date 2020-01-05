#ifndef RECIEVEDMESSAGESITERATOR_H
#define RECIEVEDMESSAGESITERATOR_H

#include "pluginapierrorcodes.h"

class RecievedMessagesIterator {
 public:
  virtual ~RecievedMessagesIterator() = default;

 public:
  virtual bool hasNext() const noexcept = 0;
  virtual bool next() noexcept = 0;

  virtual PluginApiErrorCodes error() const noexcept = 0;

  virtual const char* address() const noexcept = 0;
  virtual const char* message() const noexcept = 0;
};

#endif  // RECIEVEDMESSAGESITERATOR_H
