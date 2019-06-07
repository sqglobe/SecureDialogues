#include "dialogmessage.h"
#include <cassert>

#include "utils.h"

DialogMessage::DialogMessage(const DialogMessage::Action& action,
                             std::string content,
                             std::string dialogId,
                             std::string adress,
                             unsigned long number,
                             std::string sign) :
    mAction(action),
    mContent(std::move(content)), mDialogId(std::move(dialogId)),
    mAdress(std::move(adress)), mSign(std::move(sign)),
    mSequentialNumber(number), mTimestamp(get_timestamp()) {}

DialogMessage::DialogMessage(const DialogMessage::Action& action,
                             std::string content,
                             std::string dialogId,
                             std::string adress,
                             unsigned long number,
                             std::string sign,
                             long timestamp) :
    mAction(action),
    mContent(std::move(content)), mDialogId(std::move(dialogId)),
    mAdress(std::move(adress)), mSign(std::move(sign)),
    mSequentialNumber(number), mTimestamp(timestamp) {}

DialogMessage::Action DialogMessage::action() const noexcept {
  return mAction;
}

std::string DialogMessage::content() const noexcept {
  return mContent;
}

std::string DialogMessage::dialogId() const noexcept {
  return mDialogId;
}

std::string DialogMessage::adress() const noexcept {
  return mAdress;
}

unsigned long DialogMessage::sequential() const noexcept {
  return mSequentialNumber;
}

std::string DialogMessage::sign() const {
  return mSign;
}

long DialogMessage::timestamp() const {
  return mTimestamp;
}

void DialogMessage::setSequentialNumber(unsigned long number) noexcept {
  mSequentialNumber = number;
}

void DialogMessage::setSignature(const std::string& signature) {
  mSign = signature;
}

void DialogMessage::setContent(const std::string& content) {
  mContent = content;
}

DialogMessage make_abort(const std::string& dialogId,
                         const std::string& contact,
                         unsigned long number) {
  return DialogMessage(DialogMessage::Action::ABORT, "", dialogId, contact,
                       number);
}

DialogMessage make_ack(const DialogMessage& mess) {
  return DialogMessage(DialogMessage::Action::ACK, "", mess.dialogId(),
                       mess.adress(), mess.sequential(), " ");
}
