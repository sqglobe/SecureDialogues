#include "recievedcontactsmenu.h"
#include "wrappers/recievedcontactsstoragewrapper.h"

#include <QMenu>
#include <QMessageBox>

RecievedContactsMenu::RecievedContactsMenu(QObject* parent) :
    QObject(parent), mMenu(new QMenu) {
  mMenu->setTitle("Укажите действие с полученным контактом");
  QAction* act = mMenu->addAction("Просмотр/добавить");
  QObject::connect(act, &QAction::triggered, this,
                   &RecievedContactsMenu::onShowRecievedContact);
  act->setToolTip(
      "Щелкните для подробного просмотра полученных данных и добавления в "
      "контакты");

  act = mMenu->addAction("Удалить");
  act->setToolTip("Щелкните для удаления полученных данных");
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
