#ifndef RECIEVEDDISCOVEREDCONTACTSDIALOG_H
#define RECIEVEDDISCOVEREDCONTACTSDIALOG_H

#include <QDialog>
#include <memory>
class RecievedContactsStorageWrapper;
class DiscoveredContactModel;

namespace Ui {
class RecievedDiscoveredContactsDialog;
}

class RecievedDiscoveredContactsDialog : public QDialog {
  Q_OBJECT

 public:
  explicit RecievedDiscoveredContactsDialog(
      DiscoveredContactModel* model,
      std::shared_ptr<RecievedContactsStorageWrapper> storageWrapper,
      QWidget* parent = nullptr);
  ~RecievedDiscoveredContactsDialog();

 public slots:
  void removeRecievedContact(std::string dialogId);

 signals:
  void showDialogMenu(QPoint pos, std::string dialogId);

 private slots:
  void requestToShowMenu(QPoint pos);

 private:
  Ui::RecievedDiscoveredContactsDialog* ui;
  std::shared_ptr<RecievedContactsStorageWrapper> mStorageWrapper;
};

#endif  // RECIEVEDDISCOVEREDCONTACTSDIALOG_H
