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

std::string_view DialogMessage::content() const noexcept {
  return mContent;
}

std::string_view DialogMessage::dialogId() const noexcept {
  return mDialogId;
}

std::string_view DialogMessage::adress() const noexcept {
  return mAdress;
}

unsigned long DialogMessage::sequential() const noexcept {
  return mSequentialNumber;
}

std::string_view DialogMessage::sign() const noexcept {
  return mSign;
}

long DialogMessage::timestamp() const {
  return mTimestamp;
}

void DialogMessage::setSequentialNumber(unsigned long number) noexcept {
  mSequentialNumber = number;
}

void DialogMessage::setSignature(std::string signature) {
  mSign = std::move(signature);
}

void DialogMessage::setContent(std::string content) {
  mContent = std::move(content);
}

DialogMessage make_abort(std::string&& dialogId,
                         std::string&& contact,
                         unsigned long number) {
  return DialogMessage(DialogMessage::Action::ABORT, "", std::move(dialogId),
                       std::move(contact), number);
}

DialogMessage make_ack(const DialogMessage& mess) {
  return DialogMessage(DialogMessage::Action::ACK, "",
                       std::string(mess.dialogId()), std::string(mess.adress()),
                       mess.sequential(), " ");
}
