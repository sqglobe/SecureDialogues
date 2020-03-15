#ifndef RECIEVEDMESSAGESITERATORIMPL_H
#define RECIEVEDMESSAGESITERATORIMPL_H

#include <list>
#include <string>
#include "export/recievedmessagesiterator.h"

class RecievedMessagesIteratorImpl : public RecievedMessagesIterator {
 public:
  RecievedMessagesIteratorImpl(
      std::list<std::pair<std::string, std::string>>&& messages);
  RecievedMessagesIteratorImpl(PluginApiErrorCodes code);

 public:
  RecievedMessagesIteratorImpl(const RecievedMessagesIteratorImpl&) = delete;
  RecievedMessagesIteratorImpl(RecievedMessagesIteratorImpl&&) = delete;
  RecievedMessagesIteratorImpl& operator=(const RecievedMessagesIteratorImpl&) =
      delete;
  RecievedMessagesIteratorImpl& operator=(RecievedMessagesIteratorImpl&&) =
      delete;

 public:
  virtual bool hasNext() const noexcept override;
  virtual bool next() noexcept override;

  virtual PluginApiErrorCodes error() const noexcept override;

  virtual const char* address() const noexcept override;
  virtual const char* message() const noexcept override;

 private:
  std::list<std::pair<std::string, std::string>> mMessages;
  std::list<std::pair<std::string, std::string>>::const_iterator mCurrPos;
  PluginApiErrorCodes mCode{PluginApiErrorCodes::NoError};
};

#endif  // RECIEVEDMESSAGESITERATORIMPL_H
