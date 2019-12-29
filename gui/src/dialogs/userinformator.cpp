#include "userinformator.h"
#include <QMessageBox>

UserInformator::UserInformator(QWidget* parent) :
    QObject(nullptr), mParentWidget(parent) {
  connect(this, &UserInformator::hasMessage, this,
          &UserInformator::showMessage);
}

void UserInformator::notify(AbstractUserNotifier::Severity sev,
                            const std::string& message) {
  emit hasMessage(sev, QString(message.c_str()));
}

bool UserInformator::ask(const std::string& question) const {
  auto res = QMessageBox::question(mParentWidget, tr("Choise required"),
                                   QString(question.c_str()),
                                   QMessageBox::Apply | QMessageBox::Cancel);
  return QMessageBox::Apply == res;
}

void UserInformator::showMessage(AbstractUserNotifier::Severity sev,
                                 QString message) {
  using S = AbstractUserNotifier::Severity;
  if (S::ERROR == sev) {
    QMessageBox::critical(mParentWidget, tr("Error!"), message);
  } else if (S::WARNING == sev) {
    QMessageBox::warning(mParentWidget, tr("Attention!"), message);
  } else {
    QMessageBox::information(mParentWidget, tr("Message"), message);
  }
}
