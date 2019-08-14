#ifndef DIALOGUSERMODEL_H
#define DIALOGUSERMODEL_H

#include <QAbstractListModel>
#include <memory>
#include "dialoginfo.h"
#include "interfaces/changelistener.h"
#include "interfaces/messageconteinereventhandler.h"

#include "containers/storages.h"

class DialogManager;

/**
 * @brief Отображает список активных диалогов.
 * Применяется, чтобы пользователь мог выбрать один из диалогов и просмотреть
 * все сообщения для него.
 */
class DialogUserModel : public QAbstractListModel,
                        public ChangeListener<Dialog>,
                        public ChangeListener<Contact>,
                        public MessageContainerEventHandler {
  Q_OBJECT

 public:
  DialogUserModel(std::shared_ptr<ContactStorage> contacts,
                  const std::vector<Dialog>& dialogs);

 public:
  int rowCount(const QModelIndex& = QModelIndex()) const override;
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;

 public:
  void added(const Dialog& obj) override;
  void changed(const Dialog& obj) override;
  void removed(const Dialog& obj) override;

  void added(const Contact&) override;
  void changed(const Contact& obj) override;
  void removed(const Contact&) override;

 public:
  void messageAdded(const std::string& dialogId,
                    const std::shared_ptr<const UserMessage>&) override;

  void activeDialogChanged(const std::string&) override;
  void peekMessage(const std::shared_ptr<const UserMessage>&) override;
  void invalidateData(const std::string&) override;

 public:
  std::pair<Dialog::Status, std::string> getDialogInfo(
      const QModelIndex& pos) const;

 private:
  void updateActiveDialog(const DialogInfo& obj);

 signals:
  void newDialogWasSelected(std::string dialogId);
  void notValidDialogWasSelected(std::string dialogId);

 public slots:
  void rowWasSelected(const QModelIndex& index);

 private:
  std::vector<DialogInfo> mDialogsInfo;
  std::string activeDialog;
  std::shared_ptr<ContactStorage> mContacts;
};

#endif  // DIALOGUSERMODEL_H
