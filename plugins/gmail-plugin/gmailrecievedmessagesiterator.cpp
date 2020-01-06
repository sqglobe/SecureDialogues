#include "gmailrecievedmessagesiterator.h"
#include "oauth-agents/utils/base64.h"

GmailRecievedMessagesIterator::GmailRecievedMessagesIterator(
    std::list<std::pair<std::string, std::string> >&& messages) :
    mMessages(std::move(messages)),
    mCurrPos(mMessages.cbegin()) {}

GmailRecievedMessagesIterator::GmailRecievedMessagesIterator(
    PluginApiErrorCodes code) :
    mCode(code) {
  mCurrPos = mMessages.end();
}

bool GmailRecievedMessagesIterator::hasNext() const noexcept {
  return mCurrPos != mMessages.cend();
}

bool GmailRecievedMessagesIterator::next() noexcept {
  return ++mCurrPos != mMessages.cend();
}

PluginApiErrorCodes GmailRecievedMessagesIterator::error() const noexcept {
  return mCode;
}

const char* GmailRecievedMessagesIterator::address() const noexcept {
  return mCurrPos->first.c_str();
}

const char* GmailRecievedMessagesIterator::message() const noexcept {
  return base64_decode(mCurrPos->second).c_str();
}
