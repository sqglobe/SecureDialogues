#include "activedialogmessagemodel.h"
#include <QColor>
#include "containers/usermessage.h"
Q_DECLARE_METATYPE(std::shared_ptr<const UserMessage>);

ActiveDialogMessageModel::ActiveDialogMessageModel() {}

int ActiveDialogMessageModel::rowCount(const QModelIndex&) const {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  return static_cast<int>(mRows.size());
}

QVariant ActiveDialogMessageModel::data(const QModelIndex& index,
                                        int role) const {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  if (index.row() < 0 || index.row() >= static_cast<int>(mRows.size())) {
    return QVariant();
  } else if (role == Qt::DisplayRole) {
    auto p = static_cast<std::size_t>(index.row());
    return QVariant::fromValue(mRows[p]);
  }
  return QVariant();
}

Qt::ItemFlags ActiveDialogMessageModel::flags(const QModelIndex&) const {
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void ActiveDialogMessageModel::messageAdded(
    const std::string& dialogId,
    const std::shared_ptr<const UserMessage>& message) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  if (mActiveDialog == dialogId) {
    beginInsertRows(QModelIndex(), static_cast<int>(mRows.size()),
                    static_cast<int>(mRows.size()));
    mRows.push_back(message);
    endInsertRows();
  }
}

void ActiveDialogMessageModel::activeDialogChanged(
    const std::string& newActiveDialogId) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  beginResetModel();
  mRows.clear();
  endResetModel();
  mActiveDialog = newActiveDialogId;
}

void ActiveDialogMessageModel::peekMessage(
    const std::shared_ptr<const UserMessage>& message) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  beginInsertRows(QModelIndex(), static_cast<int>(mRows.size()),
                  static_cast<int>(mRows.size()));
  mRows.push_back(message);
  endInsertRows();
}

void ActiveDialogMessageModel::invalidateData(const std::string& dialogId) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  if (dialogId == mActiveDialog) {
    beginResetModel();
    endResetModel();
  }
}
