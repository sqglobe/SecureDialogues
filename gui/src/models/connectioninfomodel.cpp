#include "connectioninfomodel.h"
#include <QBrush>
#include <QColor>
#include "primitives/connectionholder.h"
#include "utils/gui_helpers.h"

ConnectionInfoModel::ConnectionInfoModel(
    const std::vector<ChangeListener::element>& elements) {
  std::transform(std::cbegin(elements), std::cend(elements),
                 std::back_inserter(mData),
                 [](const ChangeListener::element& element) -> ConnData {
                   return ConnData{QString(element.connName().c_str()),
                                   Channel::ChannelStatus::UNDEFINED};
                 });
}

int ConnectionInfoModel::rowCount(const QModelIndex&) const {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> lock(mMutex);
  return simpleRowCount();
}

QVariant ConnectionInfoModel::data(const QModelIndex& index, int role) const {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> lock(mMutex);
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

std::optional<std::string> ConnectionInfoModel::getId(
    const QModelIndex& index) const {
  if (index.isValid()) {
    return mData[static_cast<std::size_t>(index.row())].connName.toStdString();
  }
  return {};
}

void ConnectionInfoModel::added(const ChangeListener::element& element) {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> lock(mMutex);
  beginInsertRows(QModelIndex(), simpleRowCount(), simpleRowCount());
  mData.push_back(
      {QString(element.connName().c_str()), Channel::ChannelStatus::UNDEFINED});
  endInsertRows();
}

void ConnectionInfoModel::changed(const ChangeListener::element&) {}

void ConnectionInfoModel::removed(const ChangeListener::element& element) {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> lock(mMutex);
  auto connName = QString(element.connName().c_str());
  auto it = std::find_if(
      std::begin(mData), std::end(mData),
      [&connName](const auto& conn) { return connName == conn.connName; });
  if (it != std::end(mData)) {
    auto diff = static_cast<int>(std::distance(std::begin(mData), it));
    beginRemoveRows(QModelIndex(), diff, diff);
    mData.erase(it);
    endRemoveRows();
  }
}

void ConnectionInfoModel::updateChannelStatus(Channel::ChannelStatus status,
                                              const std::string& channelName,
                                              const std::string&) {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> lock(mMutex);
  auto it =
      std::find_if(std::begin(mData), std::end(mData),
                   [connName = QString(channelName.c_str())](const auto& conn) {
                     return connName == conn.connName;
                   });
  if (it != std::end(mData)) {
    auto diff = static_cast<int>(std::distance(std::begin(mData), it));
    it->status = status;
    emit dataChanged(createIndex(diff, 0), createIndex(diff, 0));
  }
}
