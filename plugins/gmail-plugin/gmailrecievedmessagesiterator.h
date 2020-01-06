#ifndef GMAILRECIEVEDMESSAGESITERATOR_H
#define GMAILRECIEVEDMESSAGESITERATOR_H

#include <list>
#include <string>
#include "export/recievedmessagesiterator.h"

class GmailRecievedMessagesIterator : public RecievedMessagesIterator {
 public:
  GmailRecievedMessagesIterator(
      std::list<std::pair<std::string, std::string>>&& messages);
  GmailRecievedMessagesIterator(PluginApiErrorCodes code);

 public:
  GmailRecievedMessagesIterator(const GmailRecievedMessagesIterator&) = delete;
  GmailRecievedMessagesIterator(GmailRecievedMessagesIterator&&) = delete;
  GmailRecievedMessagesIterator& operator=(
      const GmailRecievedMessagesIterator&) = delete;
  GmailRecievedMessagesIterator& operator=(GmailRecievedMessagesIterator&&) =
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

#endif  // GMAILRECIEVEDMESSAGESITERATOR_H
