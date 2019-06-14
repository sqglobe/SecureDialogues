#include "dialogstatemachine.h"
#include <cassert>

bool DialogStateMachine::isStateTransitionAllowed(Dialog::Status currState,
                                                  Dialog::Status newState) const
    noexcept {
  using S = Dialog::Status;

  switch (currState) {
    case S::NEW:
      return newState == S::WAIT_CONFIRM || newState == S::ABORTED;
    case S::CREATE_REQUEST:
      return newState == S::ACCEPTED_WAIT || newState == S::ABORTED;
    case S::WAIT_CONFIRM:
      return newState == S::ACTIVE || newState == S::ABORTED ||
             newState == S::WAIT_CONFIRM;
    case S::ACCEPTED_WAIT:
      return newState == S::WAIT_KEY_VERIFICAION || newState == S::ABORTED;
    case S::WAIT_KEY_VERIFICAION:
      return newState == S::ACTIVE || newState == S::CLOSED ||
             newState == S::ABORTED;
    case S::ACTIVE:
      return newState == S::CLOSED || newState == S::ABORTED;
    case S::CLOSED:
      [[fallthrough]];
    case S::ABORTED:
      return false;
  }
  assert(false);
}

bool DialogStateMachine::isMessageActionAllowed(
    Dialog::Status currState,
    DialogMessage::Action action) const noexcept {
  using S = Dialog::Status;
  using A = DialogMessage::Action;
  switch (currState) {
    case S::NEW:
      return action == A::CREATE_DIALOG || action == A::ABORT;
    case S::CREATE_REQUEST:
      return action == A::ABORT || action == A::VERIFY_KEY;
    case S::WAIT_CONFIRM:
      return action == A::ABORT || action == A::KEY_VERIFICATION;
    case S::ACCEPTED_WAIT:
      return action == A::ABORT;
    case S::WAIT_KEY_VERIFICAION:
      return action == A::ABORT || action == A::ACCEPT_DIALOG ||
             action == A::CLOSE_DIALOG;
    case S::ACTIVE:
      return action == A::ABORT || action == A::CLOSE_DIALOG ||
             action == A::MESSAGE;
    case S::CLOSED:
      [[fallthrough]];
    case S::ABORTED:
      return false;
  }
  assert(false);
}
