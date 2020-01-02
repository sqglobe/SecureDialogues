#include "dialogactionmenu.h"

#include <QAction>
#include <QTableView>
#include "primitives/dialog.h"

DialogActionMenu::DialogActionMenu(
    std::shared_ptr<AbstractUserNotifier> notifier,
    QObject* parent) :
    QObject(parent),
    mMenu(new QMenu()), mNotifier(std::move(notifier)) {
  mMenu->setTitle(tr("Select dialog action"));
}

void DialogActionMenu::aboutToHide() {
  mDialogId = "";
}

void DialogActionMenu::closeDialogAction() {
  if (!mDialogId.empty()) {
    emit closeDialog(mDialogId);
  }
}

void DialogActionMenu::removeAction() {
  if (!mDialogId.empty()) {
    emit removeDialog(mDialogId);
  }
}

void DialogActionMenu::showMenuAtPos(QPoint pos,
                                     Dialog::Status /*status*/,
                                     std::string dialogId) {
  mMenu->clear();
  QAction* act;
  mDialogId = dialogId;
  act = mMenu->addAction(tr("Close"));
  auto res = connect(act, &QAction::triggered, this,
                     &DialogActionMenu::closeDialogAction);

  act = mMenu->addAction(tr("Remove"));
  connect(act, &QAction::triggered, this, &DialogActionMenu::removeAction);

  mMenu->popup(pos);
}
