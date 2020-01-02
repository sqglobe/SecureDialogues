#ifndef IMPORTDISCOVEREDCONTACTDIALOG_H
#define IMPORTDISCOVEREDCONTACTDIALOG_H

#include <QDialog>
#include <memory>
#include <optional>
#include "ui_importdiscoveredcontactdialog.h"
#include "utils/translatechangeeventhandler.h"

class RecievedContactsStorageWrapper;
class ContactWidget;

class ImportDiscoveredContactDialog
    : public TranslateChangeEventHandler<QDialog,
                                         Ui::ImportDiscoveredContactDialog> {
  Q_OBJECT

 public:
  explicit ImportDiscoveredContactDialog(
      std::shared_ptr<ContactWidget> widget,
      std::shared_ptr<RecievedContactsStorageWrapper> wrapper,
      QWidget* parent = nullptr);
  ~ImportDiscoveredContactDialog();

 public slots:
  void onShowRecievedContact(std::string dialogId);

 private slots:
  void on_closeDialog_clicked();

  void on_importButton_clicked();

 private:
  Ui::ImportDiscoveredContactDialog* ui;

 private:
  std::shared_ptr<ContactWidget> mWidget;
  std::shared_ptr<RecievedContactsStorageWrapper> mWrapper;
  std::optional<std::string> mDialogId;
};

#endif  // IMPORTDISCOVEREDCONTACTDIALOG_H
