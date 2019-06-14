#ifndef DIALOGSTATEMACHINE_H
#define DIALOGSTATEMACHINE_H

#include "primitives/dialog.h"
#include "primitives/dialogmessage.h"

#include <set>
#include <vector>

/**
 * @brief Машина состояний диалога.
 * Позволяет определить является ли:
 *  1. переход из одного состояния в другое допустимым или нет.
 *  2. создание сообщения с определенным типом в указанном состоянии допустимым.
 */
class DialogStateMachine {
 public:
 public:
  bool isStateTransitionAllowed(Dialog::Status currState,
                                Dialog::Status newState) const noexcept;
  bool isMessageActionAllowed(Dialog::Status currState,
                              DialogMessage::Action action) const noexcept;
};

#endif  // DIALOGSTATEMACHINE_H
