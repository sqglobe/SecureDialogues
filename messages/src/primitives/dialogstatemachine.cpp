#include "dialogstatemachine.h"

// enum class Status{NEW = 0, CREATE_REQUEST, WAIT_CONFIRM, ACCEPTED_WAIT,
// CANCEL_WAIT, WAIT_KEY_VERIFICAION, ACTIVE,  CLOSED, CANCELED, ABORTED};
DialogStateMachine::DialogStateMachine() {
  using S = Dialog::Status;
  using A = DialogMessage::Action;
  mStateTransisions = {
      {S::WAIT_CONFIRM, S::ABORTED},                          // NEW
      {S::ACCEPTED_WAIT, S::CANCEL_WAIT, S::ABORTED},         // CREATE_REQUEST
      {S::ACTIVE, S::CANCELED, S::ABORTED, S::WAIT_CONFIRM},  // WAIT_CONFIRM
      {S::WAIT_KEY_VERIFICAION, S::ABORTED},                  // ACCEPTED_WAIT
      {S::CANCELED, S::ABORTED},                              // CANCEL_WAIT
      {S::ACTIVE, S::CLOSED, S::ABORTED},  // WAIT_KEY_VERIFICAION
      {S::CLOSED, S::ABORTED},             // ACTIVE
      {},                                  // CLOSED
      {},                                  // CANCELED
      {},                                  // ABORTED
  };

  mAllowedActions = {
      {A::CREATE_DIALOG, A::ABORT},                   // NEW
      {A::CANCEL_DIALOG, A::ABORT, A::VERIFY_KEY},    // CREATE_REQUEST
      {A::ABORT, A::KEY_VERIFICATION},                // WAIT_CONFIRM
      {A::ABORT},                                     // ACCEPTED_WAIT
      {A::ABORT},                                     // CANCEL_WAIT
      {A::ABORT, A::ACCEPT_DIALOG, A::CLOSE_DIALOG},  // WAIT_KEY_VERIFICAION
      {A::CLOSE_DIALOG, A::ABORT, A::MESSAGE},        // ACTIVE
      {},                                             // CLOSED
      {},                                             // CANCELED
      {},                                             // ABORTED
  };
}

bool DialogStateMachine::isStateTransitionAllowed(Dialog::Status currState,
                                                  Dialog::Status newState) const
    noexcept {
  return mStateTransisions[static_cast<std::size_t>(currState)].count(
             newState) > 0;
}

bool DialogStateMachine::isMessageActionAllowed(
    Dialog::Status currState,
    DialogMessage::Action action) const noexcept {
  return mAllowedActions[static_cast<std::size_t>(currState)].count(action) > 0;
}
