#include "dialog.h"
#include "contact.h"

#include <algorithm>   //for std::generate_n
#include <functional>  //for std::function
#include <random>
#include <vector>
#include "utils.h"

#include "dialogstatemachine.h"

#include <random>

static const DialogStateMachine DIALOG_STATE_MACHINE;

static unsigned long init_sequental() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<unsigned long> dis(
      0, std::numeric_limits<unsigned int>::max());
  return dis(gen);
}

invalid_dialog_action::invalid_dialog_action(const std::string& str) :
    std::runtime_error(str) {}

Dialog::Dialog(const Contact& contact) :
    Dialog(contact.id(),
           contact.adress(),
           contact.name(),
           make_uiid(),
           0,
           init_sequental(),
           Dialog::Status::NEW) {}

Dialog::Dialog(std::string contactId,
               std::string address,
               std::string name,
               std::string dialogId,
               unsigned long peerSequental,
               unsigned long thisSequental,
               Dialog::Status status) :
    mDialogId(std::move(dialogId)),
    mContactId(std::move(contactId)), mAdress(std::move(address)),
    mName(std::move(name)), mStatus(status), mLastSequental(peerSequental),
    mThisSequental(thisSequental) {}

Dialog::Dialog(const Contact& contact,
               std::string dialogId,
               unsigned long sequental,
               Dialog::Status status) :
    Dialog(contact.id(),
           contact.adress(),
           contact.name(),
           dialogId,
           sequental,
           init_sequental(),
           status) {}

std::string Dialog::getDialogId() const {
  return mDialogId;
}

DialogMessage Dialog::makeMessage(DialogMessage::Action action,
                                  const std::string& content) const
    noexcept(false) {
  if (!DIALOG_STATE_MACHINE.isMessageActionAllowed(mStatus, action)) {
    std::stringstream ss;
    ss << "Message with action " << static_cast<int>(action)
       << " cant be made for dialog state " << static_cast<int>(mStatus);
    throw invalid_dialog_action(ss.str());
  }
  return DialogMessage(action, content, mDialogId, mAdress, getNextSequental());
}

DialogMessage Dialog::makeAbort() {
  setStatus(Status::ABORTED);
  auto message = make_abort(mDialogId, mAdress, getNextSequental());
  return message;
}

Dialog::Status Dialog::getStatus() const noexcept {
  return mStatus;
}

void Dialog::setStatus(Dialog::Status status) noexcept(false) {
  if (!DIALOG_STATE_MACHINE.isStateTransitionAllowed(mStatus, status)) {
    std::stringstream ss;
    ss << "Dialog status transision from state " << static_cast<int>(mStatus)
       << " to " << static_cast<int>(status) << " is not allowed";
  }
  mStatus = status;
}

std::string Dialog::getContactId() const {
  return mContactId;
}

std::string Dialog::getAdress() const {
  return mAdress;
}

std::string Dialog::getName() const {
  return mAdress;
}

bool Dialog::isSequentalOk(unsigned long sequental) {
  if (sequental > mLastSequental) {
    mLastSequental = sequental;
    return true;
  }
  return false;
}

bool Dialog::isMessageActionAllowed(DialogMessage::Action action) const
    noexcept {
  return DIALOG_STATE_MACHINE.isMessageActionAllowed(mStatus, action);
}

unsigned long Dialog::getNextSequental() const {
  return ++mThisSequental;
}
