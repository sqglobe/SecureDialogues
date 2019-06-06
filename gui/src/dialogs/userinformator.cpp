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
  auto res = QMessageBox::question(mParentWidget, "Необходим выбор",
                                   QString(question.c_str()),
                                   QMessageBox::Apply | QMessageBox::Cancel);
  return QMessageBox::Apply == res;
}

void UserInformator::showMessage(AbstractUserNotifier::Severity sev,
                                 QString message) {
  using S = AbstractUserNotifier::Severity;
  if (S::ERROR == sev) {
    QMessageBox::critical(mParentWidget, "Ошибка!", message);
  } else if (S::WARNING == sev) {
    QMessageBox::warning(mParentWidget, "Внимание!", message);
  } else {
    QMessageBox::information(mParentWidget, "Сообщение", message);
  }
}
