#include "recievedcontactsmenu.h"
#include "wrappers/recievedcontactsstoragewrapper.h"

#include <QMenu>
#include <QMessageBox>

RecievedContactsMenu::RecievedContactsMenu(QObject* parent) :
    QObject(parent), mMenu(new QMenu) {
  mMenu->setTitle(tr("Select action for recieved contact information"));
  QAction* act = mMenu->addAction(tr("View/Import"));
  QObject::connect(act, &QAction::triggered, this,
                   &RecievedContactsMenu::onShowRecievedContact);
  act->setToolTip(
      tr("Click for detailed view recieved contact information and import to "
      "your contacts"));

  act = mMenu->addAction(tr("Remove"));
  act->setToolTip(tr("Click to remove selected item"));
  QObject::connect(act, &QAction::triggered, this,
                   &RecievedContactsMenu::onRemoveRecievedContact);
}

void RecievedContactsMenu::showMenuAtPos(QPoint pos, std::string dialogId) {
  mCoveredDialogId = dialogId;
  mMenu->popup(pos);
}

void RecievedContactsMenu::onShowRecievedContact() {
  if (mCoveredDialogId) {
    emit showRecievedContact(mCoveredDialogId.value());
    mCoveredDialogId.reset();
  }
}

void RecievedContactsMenu::onRemoveRecievedContact() {
  if (mCoveredDialogId) {
    emit removeRecievedContact(mCoveredDialogId.value());
    mCoveredDialogId.reset();
  }
}
