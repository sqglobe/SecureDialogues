#include "recievedmessagesiteratorimpl.h"
#include "base64.h"

RecievedMessagesIteratorImpl::RecievedMessagesIteratorImpl(
    std::list<std::pair<std::string, std::string> >&& messages) :
    mMessages(std::move(messages)),
    mCurrPos(mMessages.cbegin()) {}

RecievedMessagesIteratorImpl::RecievedMessagesIteratorImpl(
    PluginApiErrorCodes code) :
    mCode(code) {
  mCurrPos = mMessages.end();
}

bool RecievedMessagesIteratorImpl::hasNext() const noexcept {
  return mCurrPos != mMessages.cend();
}

bool RecievedMessagesIteratorImpl::next() noexcept {
  return ++mCurrPos != mMessages.cend();
}

PluginApiErrorCodes RecievedMessagesIteratorImpl::error() const noexcept {
  return mCode;
}

const char* RecievedMessagesIteratorImpl::address() const noexcept {
  return mCurrPos->first.c_str();
}

const char* RecievedMessagesIteratorImpl::message() const noexcept {
  return base64_decode(mCurrPos->second).c_str();
}
