#ifndef PUBLICKEYDIALOG_H
#define PUBLICKEYDIALOG_H

#include <QDialog>

#include <memory>
class CryptoSystemImpl;

namespace Ui {
class PublicKeyDialog;
}

class PublicKeyDialog : public QDialog {
  Q_OBJECT

 public:
  explicit PublicKeyDialog(std::shared_ptr<CryptoSystemImpl> system,
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
};

#endif  // PUBLICKEYDIALOG_H
