#ifndef DIALOGUSERMODEL_H
#define DIALOGUSERMODEL_H

#include <QAbstractListModel>
#include <memory>
#include "dialoginfo.h"
#include "interfaces/changewatcher.h"
#include "interfaces/messageconteinereventhandler.h"

class DialogManager;

/**
 * @brief Отображает список активных диалогов.
 * Применяется, чтобы пользователь мог выбрать один из диалогов и просмотреть
 * все сообщения для него.
 */
class DialogUserModel : public QAbstractListModel,
                        public ChangeWatcher<std::shared_ptr<const Dialog>>,
                        public MessageContainerEventHandler {
  Q_OBJECT

 public:
  explicit DialogUserModel(const std::shared_ptr<DialogManager>& dialogs);

 public:
  virtual int rowCount(const QModelIndex& = QModelIndex()) const override;
  virtual QVariant data(const QModelIndex& index,
                        int role = Qt::DisplayRole) const override;

 public:
  virtual void added(const element& obj) override;
  virtual void changed(const element& obj) override;
  virtual void removed(const element& obj) override;

 public:
  virtual void messageAdded(const std::string& dialogId,
                            const std::shared_ptr<const UserMessage>&) override;

  virtual void activeDialogChanged(const std::string&) override;
  virtual void peekMessage(const std::shared_ptr<const UserMessage>&) override;
  virtual void invalidateData(const std::string&) override;

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
};

#endif  // DIALOGUSERMODEL_H
