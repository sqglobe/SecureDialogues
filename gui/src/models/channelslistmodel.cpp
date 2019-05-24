#include "channelslistmodel.h"
#include <algorithm>
#include <iterator>
#include "primitives/connectionholder.h"

#include <QBrush>
#include <QColor>

#include <iostream>

ChannelsListModel::ChannelsListModel(
    const std::vector<ConnectionHolder>& elements) {
  std::transform(elements.cbegin(), elements.cend(),
                 std::back_inserter(mChannelNames),
                 [](const ConnectionHolder& cInfo) -> ListItem {
                   return {cInfo.getConnectionName().c_str(),
                           "Подключение на данный момент не установлено",
                           Channel::ChannelStatus::UNDEFINED};
                 });
}

int ChannelsListModel::rowCount(const QModelIndex&) const {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  return mChannelNames.size();
}

QVariant ChannelsListModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid())
    return QVariant();
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  if (Qt::DisplayRole == role) {
    return mChannelNames.at(index.row()).name;
  } else if (Qt::ToolTipRole == role) {
    return mChannelNames.at(index.row()).message;
  } else if (Qt::BackgroundRole == role) {
    using S = Channel::ChannelStatus;
    switch (mChannelNames.at(index.row()).status) {
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

void ChannelsListModel::added(const ChangeWatcher::element& obj) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  beginInsertRows(QModelIndex(), mChannelNames.size(), mChannelNames.size());
  mChannelNames.append({obj.getConnectionName().c_str(),
                        "Подключение на данный момент не установлено",
                        Channel::ChannelStatus::UNDEFINED});
  endInsertRows();
}

void ChannelsListModel::changed(const ChangeWatcher::element& obj) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto it = std::find_if(mChannelNames.begin(), mChannelNames.end(),
                         [search = QString(obj.getConnectionName().c_str())](
                             auto const& name) { return name.name == search; });
  if (mChannelNames.end() != it) {
    int dst = static_cast<int>(std::distance(mChannelNames.begin(), it));
    it->status = obj.getStatus();
    it->message = obj.getMessage().c_str();
    emit dataChanged(createIndex(dst, 0), createIndex(dst, 0));
  }
}

void ChannelsListModel::removed(const ChangeWatcher::element& obj) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto it = std::find_if(mChannelNames.begin(), mChannelNames.end(),
                         [search = QString(obj.getConnectionName().c_str())](
                             auto const& name) { return name.name == search; });
  if (mChannelNames.end() != it) {
    int dst = static_cast<int>(std::distance(mChannelNames.begin(), it));
    beginRemoveRows(QModelIndex(), dst, dst);
    mChannelNames.erase(it);
    endRemoveRows();
  }
}
