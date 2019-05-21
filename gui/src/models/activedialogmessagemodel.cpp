#include "activedialogmessagemodel.h"

#include <QColor>
#include "containers/usermessage.h"

char leading_symbol(UserMessage::Type type) {
  using T = UserMessage::Type;
  switch (type) {
    case T::IN:
      return '>';
    case T::OUT:
      return '<';
    default:
      return '\0';
  }
}

ActiveDialogMessageModel::ActiveDialogMessageModel() {}

int ActiveDialogMessageModel::rowCount(const QModelIndex&) const {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  return static_cast<int>(mRows.size());
}

QVariant ActiveDialogMessageModel::data(const QModelIndex& index,
                                        int role) const {
  using size_type = std::vector<std::shared_ptr<const UserMessage> >::size_type;
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  if (index.row() < 0 || index.row() >= static_cast<int>(mRows.size())) {
    return QVariant();
  } else if (Qt::DisplayRole == role) {
    QString item(
        leading_symbol(mRows.at(static_cast<size_type>(index.row()))->type()));
    item.append(" ").append(
        mRows.at(static_cast<size_type>(index.row()))->content().c_str());
    return item;
  } else if (Qt::ForegroundRole == role) {
    switch (mRows.at(static_cast<size_type>(index.row()))->status()) {
      case UserMessage::Status::DELIVERIED:
        return QColor(Qt::black);
      case UserMessage::Status::NOT_DELIVERIED:
        return QColor(Qt::red);
      case UserMessage::Status::WAIT_DELIVERY:
        return QColor(Qt::gray);
      case UserMessage::Status::ERROR:
        return QColor(Qt::darkRed);
    }
  } else if (Qt::TextAlignmentRole == role) {
    using T = UserMessage::Type;
    switch (mRows.at(static_cast<size_type>(index.row()))->type()) {
      case T::IN:
        return Qt::AlignLeft;
      case T::OUT:
        return Qt::AlignRight;
      case T::SYSTEM:
        return Qt::AlignCenter;
    }
  }
  return QVariant();
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
