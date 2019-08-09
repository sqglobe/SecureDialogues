#include "dialog.h"
#include "contact.h"

#include <algorithm>   //for std::generate_n
#include <functional>  //for std::function
#include <random>
#include <vector>

#include "dialogstatemachine.h"

#include <random>

static const DialogStateMachine DIALOG_STATE_MACHINE;

unsigned long init_sequental() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<unsigned long> dis(
      0, std::numeric_limits<unsigned int>::max());
  return dis(gen);
}

invalid_dialog_action::invalid_dialog_action(const std::string& str) :
    std::runtime_error(str) {}

std::string_view Dialog::getDialogId() const noexcept {
  return mDialogId;
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

std::string_view Dialog::getContactId() const noexcept {
  return mContactId;
}

unsigned long Dialog::getPeerSequental() const noexcept {
  return mPeerSequental;
}

unsigned long Dialog::getThisSequental() const noexcept {
  return mThisSequental;
}

bool Dialog::isSequentalOk(unsigned long sequental) {
  if (sequental > mPeerSequental) {
    mPeerSequental = sequental;
    return true;
  }
  return false;
}

bool Dialog::isMessageActionAllowed(DialogMessage::Action action) const
    noexcept {
  return DIALOG_STATE_MACHINE.isMessageActionAllowed(mStatus, action);
}

unsigned long Dialog::makeNextSequental() {
  return ++mThisSequental;
}
