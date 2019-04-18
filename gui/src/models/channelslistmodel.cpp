#include "channelslistmodel.h"
#include <algorithm>
#include <iterator>
#include "primitives/connectionholder.h"

ChannelsListModel::ChannelsListModel(
    const std::vector<ConnectionHolder>& elements) {
  std::transform(elements.cbegin(), elements.cend(),
                 std::back_inserter(mChannelNames),
                 [](const ConnectionHolder& cInfo) -> QString {
                   return QString(cInfo.getConnectionName().c_str());
                 });
}

int ChannelsListModel::rowCount(const QModelIndex&) const {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  return mChannelNames.size();
}

QVariant ChannelsListModel::data(const QModelIndex& index, int role) const {
  if (Qt::DisplayRole == role && index.isValid()) {
    std::lock_guard<std::recursive_mutex> guard(mMutex);
    return mChannelNames.at(index.row());
  }
  return QVariant();
}

void ChannelsListModel::added(const ChangeWatcher::element& obj) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  beginInsertRows(QModelIndex(), mChannelNames.size(), mChannelNames.size());
  mChannelNames.append(obj.getConnectionName().c_str());
  endInsertRows();
}

void ChannelsListModel::changed(const ChangeWatcher::element&) {
  // not needed, becouse channel name is const
}

void ChannelsListModel::removed(const ChangeWatcher::element& obj) {
  std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto it = std::find(mChannelNames.begin(), mChannelNames.end(),
                      QString(obj.getConnectionName().c_str()));
  if (mChannelNames.end() != it) {
    int dst = std::distance(mChannelNames.begin(), it);
    beginRemoveRows(QModelIndex(), dst, dst);
    mChannelNames.erase(it);
    endRemoveRows();
  }
}
