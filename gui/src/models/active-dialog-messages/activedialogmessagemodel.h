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
  ActiveDialogMessageModel() = default;

 public:
  int rowCount(const QModelIndex& = QModelIndex()) const override;
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex&) const override;

 public:
  void messageAdded(const std::string& dialogId,
                    const std::shared_ptr<const UserMessage>& message) override;
  void activeDialogChanged(const std::string& newActiveDialogId) override;
  void peekMessage(const std::shared_ptr<const UserMessage>& message) override;
  void invalidateData(const std::string& dialogId) override;

 private:
  std::vector<std::shared_ptr<const UserMessage> > mRows;
  std::string mActiveDialog;
  mutable std::recursive_mutex mMutex;
};

#endif  // ACTIVEDIALOGMESSAGEMODEL_H
