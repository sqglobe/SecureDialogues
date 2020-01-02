#ifndef RECIEVEDDISCOVEREDCONTACTSDIALOG_H
#define RECIEVEDDISCOVEREDCONTACTSDIALOG_H

#include <QDialog>
#include <memory>
#include "ui_recieveddiscoveredcontactsdialog.h"
#include "utils/translatechangeeventhandler.h"

class RecievedContactsStorageWrapper;
class DiscoveredContactModel;

class RecievedDiscoveredContactsDialog
    : public TranslateChangeEventHandler<QDialog,
                                         Ui::RecievedDiscoveredContactsDialog> {
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
