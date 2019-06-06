#ifndef DIALOGACTIONMENU_H
#define DIALOGACTIONMENU_H

#include <QMenu>
#include <QObject>
#include <QPoint>
#include <memory>
#include "primitives/dialog.h"

class AbstractUserNotifier;

/**
 * @brief Меню, которое содержит список действий для выбранного диалога.
 * Используется в главном меню, чтобы пользователь мог выбрать, какие действия
 * выполнить с диалогом - закрыть, отклонить и пр.
 */
class DialogActionMenu : public QObject {
  Q_OBJECT
 public:
  explicit DialogActionMenu(
      const std::shared_ptr<AbstractUserNotifier>& notifier,
      QObject* parent = nullptr);

 signals:
  void acceptDialog(std::string dialogId);
  void canceltDialog(std::string dialogId);
  void closeDialog(std::string dialogId);
  void removeDialog(std::string dialogId);

 protected slots:
  void aboutToHide();
  void acceptAction();
  void closeDialogAction();
  void cancelActoin();
  void removeAction();

 public slots:

  /**
   * @brief Отображает меню для указанного диалога
   * От диалога зависит состав пунктов меню
   * @param pos позиция для меню
   * @param status статус диалога, для которого показывается сообщение
   * @param dialogId идентификатор диалога, для которого нужно отобразить диалог
   */
  void showMenuAtPos(QPoint pos, Dialog::Status status, std::string dialogId);

 private:
  std::string mDialogId;
  std::unique_ptr<QMenu> mMenu;
  std::shared_ptr<AbstractUserNotifier> mNotifier;
};

#endif  // DIALOGACTIONMENU_H
