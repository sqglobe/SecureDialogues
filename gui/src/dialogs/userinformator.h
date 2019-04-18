#ifndef USERINFORMATOR_H
#define USERINFORMATOR_H

#include <QObject>
#include <QWidget>
#include "interfaces/abstractuserask.h"
#include "interfaces/abstractusernotifier.h"

/**
 * @brief Отображает пользователю информационное сообщение или запрашивает
 * подтверждение выполнения действия
 */
class UserInformator : public QObject,
                       public AbstractUserNotifier,
                       public AbstractUserAsk {
  Q_OBJECT
 public:
  explicit UserInformator(QWidget* parent);

 public:
  virtual void notify(Severity sev, const std::string& message) override;
  virtual bool ask(const std::string& question) const override;

 signals:
  void hasMessage(AbstractUserNotifier::Severity sev, QString message);
 private slots:
  void showMessage(AbstractUserNotifier::Severity sev, QString message);

 private:
  QWidget* mParentWidget;
};

#endif  // USERINFORMATOR_H
