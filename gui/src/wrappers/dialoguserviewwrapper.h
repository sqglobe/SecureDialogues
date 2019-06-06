#ifndef DIALOGUSERVIEWWRAPPER_H
#define DIALOGUSERVIEWWRAPPER_H

#include <QObject>

#include <memory>

#include <QPoint>
#include <QSortFilterProxyModel>
#include "primitives/dialog.h"

class QListView;
class DialogUserModel;

/**
 * @brief Извлекает из модели диалог и передает его в DialogActionMenu
 */
class DialogUserViewWrapper : public QObject {
  Q_OBJECT
 public:
  explicit DialogUserViewWrapper(QListView* view,
                                 std::shared_ptr<DialogUserModel> model,
                                 QObject* parent = nullptr);

 signals:
  void showMenuForDialog(QPoint pos,
                         Dialog::Status status,
                         std::string dialogId);

 public slots:
  void requestToShowMenu(QPoint pos);
  void requestToActivateItem(const QModelIndex& index);

 private:
  QListView* mView;
  std::unique_ptr<QSortFilterProxyModel> mProxy;
  std::shared_ptr<DialogUserModel> mModel;
};

#endif  // DIALOGUSERVIEWWRAPPER_H
