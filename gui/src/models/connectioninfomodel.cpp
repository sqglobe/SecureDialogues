#include "connectioninfomodel.h"
#include <QBrush>
#include <QColor>
#include "primitives/connectionholder.h"

int ConnectionInfoModel::rowCount(const QModelIndex&) const {
  return static_cast<int>(mData.size());
}

QVariant ConnectionInfoModel::data(const QModelIndex& index, int role) const {
  if (Qt::DisplayRole == role && index.row() >= 0 && index.row() < rowCount()) {
    return mData[static_cast<std::size_t>(index.row())].connName;
  } else if (Qt::BackgroundRole == role) {
    using S = Channel::ChannelStatus;
    switch (mData[static_cast<std::size_t>(index.row())].status) {
      case S::UNDEFINED:
        return QBrush(QColor(192, 192, 192));
      case S::CONNECTED:
        return QBrush(QColor(152, 251, 152));
      case S::FAILED_CONNECT:
        return QBrush(QColor(255, 192, 203));
      case S::AUTHORIZATION_FAILED:
        return QBrush(QColor(250, 128, 114));
    }
  }
  return QVariant();
}

void ConnectionInfoModel::added(const ChangeWatcher::element& obj) {
  auto newPos = mContainer->posOf(get_id(obj));
  beginInsertRows(QModelIndex(), newPos, newPos);
  endInsertRows();
}

void ConnectionInfoModel::changed(const ChangeWatcher::element& obj) {
  auto newPos = mContainer->posOf(get_id(obj));
  emit dataChanged(createIndex(newPos, 0), createIndex(newPos, 0));
}

void ConnectionInfoModel::operator()(prstorage::EnqueuedEvents event,
                                     const ConnectionHolder& holder) {
  if (event == prstorage::EnqueuedEvents::ADDED) {
    beginInsertRows(QModelIndex(), mData.size(), mData.size());
    endInsertRows();
  }
}

void ConnectionInfoModel::removed([
    [maybe_unused]] const ChangeWatcher::element& obj) {
  beginResetModel();
  endResetModel();
}

ConnectionInfoContainer::const_element ConnectionInfoModel::getAt(
    const QModelIndex& pos) const {
  return mContainer->at(pos.row());
}
