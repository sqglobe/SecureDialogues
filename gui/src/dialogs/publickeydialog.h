#ifndef PUBLICKEYDIALOG_H
#define PUBLICKEYDIALOG_H

#include <QDialog>

#include <memory>
class CryptoSystemImpl;
class AbstractUserAsk;
class AbstractUserNotifier;

namespace Ui {
class PublicKeyDialog;
}

class PublicKeyDialog : public QDialog {
  Q_OBJECT

 public:
  explicit PublicKeyDialog(std::shared_ptr<CryptoSystemImpl> system,
                           std::shared_ptr<AbstractUserNotifier> notifier,
                           std::shared_ptr<const AbstractUserAsk> ask,
                           QWidget* parent = nullptr);
  ~PublicKeyDialog() override;

 private slots:
  void generateKey();

 protected:
  void showEvent(QShowEvent* event) override;

 private:
  Ui::PublicKeyDialog* ui;

 private:
  std::shared_ptr<CryptoSystemImpl> mSystem;
  std::shared_ptr<AbstractUserNotifier> mNotifier;
  std::shared_ptr<const AbstractUserAsk> mAsk;
};

#endif  // PUBLICKEYDIALOG_H
