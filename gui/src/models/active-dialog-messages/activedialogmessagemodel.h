#ifndef ACTIVEDIALOGMESSAGEMODEL_H
#define ACTIVEDIALOGMESSAGEMODEL_H

#include <QAbstractListModel>
#include <mutex>
#include <utility>
#include <vector>
#include "interfaces/messageconteinereventhandler.h"

/**
 * @brief Класс используется для отображения сообщений активного диалога.
 * Под активным подразумевается диалог, для которого пользователь просматривает
 * сообщения
 */
class ActiveDialogMessageModel : public QAbstractListModel,
                                 public MessageContainerEventHandler {
 public:
  ActiveDialogMessageModel();

 public:
  virtual int rowCount(const QModelIndex& = QModelIndex()) const override;
  virtual QVariant data(const QModelIndex& index,
                        int role = Qt::DisplayRole) const override;
  virtual Qt::ItemFlags flags(const QModelIndex&) const override;

 public:
  virtual void messageAdded(
      const std::string& dialogId,
      const std::shared_ptr<const UserMessage>& message) override;
  virtual void activeDialogChanged(
      const std::string& newActiveDialogId) override;
  virtual void peekMessage(
      const std::shared_ptr<const UserMessage>& message) override;
  virtual void invalidateData(const std::string& dialogId) override;

 private:
  std::vector<std::shared_ptr<const UserMessage> > mRows;
  std::string mActiveDialog;
  mutable std::recursive_mutex mMutex;
};

#endif  // ACTIVEDIALOGMESSAGEMODEL_H
