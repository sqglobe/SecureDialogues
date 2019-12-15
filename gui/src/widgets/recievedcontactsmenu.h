#ifndef RECIEVEDCONTACTSMENU_H
#define RECIEVEDCONTACTSMENU_H

#include <QMenu>
#include <QObject>
#include <QPoint>
#include <memory>
#include <optional>
class RecievedContactsStorageWrapper;

class RecievedContactsMenu : public QObject {
  Q_OBJECT

 public:
  explicit RecievedContactsMenu(QObject* parent = nullptr);

 signals:
  void showRecievedContact(std::string dialogId);
  void removeRecievedContact(std::string dialogId);

 public slots:
  void showMenuAtPos(QPoint pos, std::string dialogId);

 private slots:
  void onRemoveRecievedContact();
  void onShowRecievedContact();

 private:
  std::optional<std::string> mCoveredDialogId;
  std::unique_ptr<QMenu> mMenu;
};

#endif  // RECIEVEDCONTACTSMENU_H
