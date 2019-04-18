#include "dialogactionmenu.h"

#include <QAction>
#include <QTableView>
#include "primitives/dialog.h"

DialogActionMenu::DialogActionMenu(
    std::shared_ptr<AbstractUserNotifier> notifier,
    QObject* parent) :
    QObject(parent),
    mMenu(new QMenu()), mNotifier(notifier) {
  mMenu->setTitle("Укажите действие для диалога");
}

void DialogActionMenu::aboutToHide() {
  mDialogId = "";
}

void DialogActionMenu::acceptAction() {
  if (!mDialogId.empty()) {
    emit acceptDialog(mDialogId);
  }
}

void DialogActionMenu::closeDialogAction() {
  if (!mDialogId.empty()) {
    emit closeDialog(mDialogId);
  }
}

void DialogActionMenu::cancelActoin() {
  if (!mDialogId.empty()) {
    emit canceltDialog(mDialogId);
  }
}

void DialogActionMenu::removeAction() {
  if (!mDialogId.empty()) {
    emit removeDialog(mDialogId);
  }
}

void DialogActionMenu::showMenuAtPos(QPoint pos,
                                     Dialog::Status status,
                                     std::string dialogId) {
  using S = Dialog::Status;
  mMenu->clear();
  QAction* act;
  mDialogId = dialogId;
  if (S::CREATE_REQUEST == status) {
    act = mMenu->addAction("Подтвердить создание");
    connect(act, &QAction::triggered, this, &DialogActionMenu::acceptAction);
    act = mMenu->addAction("Отменить создание");
    connect(act, &QAction::triggered, this, &DialogActionMenu::cancelActoin);
  }
  act = mMenu->addAction("Закрыть");
  auto res = connect(act, &QAction::triggered, this,
                     &DialogActionMenu::closeDialogAction);

  act = mMenu->addAction("Удалить");
  connect(act, &QAction::triggered, this, &DialogActionMenu::removeAction);

  mMenu->popup(pos);
}
