#include "channelslistmodel.h"
#include <algorithm>
#include <iterator>

#include <QBrush>
#include <QColor>

#include <iostream>
Q_DECLARE_METATYPE(ChannelsListModel::ListItem);

ChannelsListModel::ChannelsListModel(
    const std::vector<ConnectionHolder>& elements) {
  std::transform(elements.cbegin(), elements.cend(),
                 std::back_inserter(mChannelNames),
                 [](const ConnectionHolder& cInfo) -> ListItem {
                   return {cInfo.getConnectionName().c_str(),
                           "Подключение на данный момент не установлено",
                           Channel::ChannelStatus::UNDEFINED, cInfo.getType()};
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
  } else if (Qt::UserRole == role) {
    return QVariant::fromValue(mChannelNames.at(index.row()));
  }
  return QVariant();
}

void ChannelsListModel::operator()(prstorage::EnqueuedEvents event,
                                   const ConnectionHolder& holder) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  if (event == prstorage::EnqueuedEvents::ADDED) {
    beginInsertRows(QModelIndex(), mChannelNames.size(), mChannelNames.size());
    mChannelNames.append({holder.getConnectionName().c_str(),
                          "Подключение на данный момент не установлено",
                          Channel::ChannelStatus::UNDEFINED, holder.getType()});
    endInsertRows();
  } else if (event == prstorage::EnqueuedEvents::UPDATED) {
    auto it =
        std::find_if(mChannelNames.begin(), mChannelNames.end(),
                     [search = QString(holder.getConnectionName().c_str())](
                         auto const& name) { return name.name == search; });
    if (mChannelNames.end() != it) {
      int dst = static_cast<int>(std::distance(mChannelNames.begin(), it));
      it->status = holder.getStatus();
      it->message = holder.getMessage().c_str();
      it->connectionType = holder.getType();
      emit dataChanged(createIndex(dst, 0), createIndex(dst, 0));
    }
  } else if (event == prstorage::EnqueuedEvents::DELETED) {
    auto it =
        std::find_if(mChannelNames.begin(), mChannelNames.end(),
                     [search = QString(holder.getConnectionName().c_str())](
                         auto const& name) { return name.name == search; });
    if (mChannelNames.end() != it) {
      int dst = static_cast<int>(std::distance(mChannelNames.begin(), it));
      beginRemoveRows(QModelIndex(), dst, dst);
      mChannelNames.erase(it);
      endRemoveRows();
    }
  }
}
