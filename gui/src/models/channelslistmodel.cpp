#include "channelslistmodel.h"
#include <algorithm>
#include <iterator>

#include <QApplication>
#include <QBrush>
#include <QColor>
#include <iostream>
#include "export/pluginaddressvalidator.h"
#include "export/pluginconnectioninfo.h"
#include "plugininterface.h"

Q_DECLARE_METATYPE(ChannelsListModel::ListItem)

ChannelsListModel::ChannelsListModel(
    const std::vector<ConnectionHolder>& elements,
    std::shared_ptr<const plugin_support::PluginsContainer> container) :
    mContainer(std::move(container)) {
  std::transform(
      elements.cbegin(), elements.cend(), std::back_inserter(mChannelNames),
      [& cont = this->mContainer](const ConnectionHolder& cInfo) -> ListItem {
        const auto interface =
            cInfo.pluginConnInfo() == nullptr
                ? nullptr
                : cont->get(cInfo.pluginConnInfo()->getPluginId());
        return {cInfo.connName().c_str(),
                QApplication::tr("Connection not established yet"),
                Channel::ChannelStatus::UNDEFINED,
                interface ? interface->getAddressValidator()
                          : std::shared_ptr<const PluginAddressValidator>{}};
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
      case S::BAD_CHANNEL:
        return QBrush(QColor(229, 128, 255));
    }
  } else if (Qt::UserRole == role) {
    return QVariant::fromValue(mChannelNames.at(index.row()));
  }
  return QVariant();
}

void ChannelsListModel::added(const ChangeListener::element& element) {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> guard(mMutex);
  beginInsertRows(QModelIndex(), mChannelNames.size(), mChannelNames.size());
  const auto interface =
      mContainer->get(element.pluginConnInfo()->getPluginId());
  mChannelNames.append(
      {element.connName().c_str(),
       QApplication::tr("Connection not established yet"),
       Channel::ChannelStatus::UNDEFINED,
       interface ? interface->getAddressValidator()
                 : std::shared_ptr<const PluginAddressValidator>{}});
  endInsertRows();
}

void ChannelsListModel::changed(const ChangeListener::element& element) {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto it = std::find_if(mChannelNames.begin(), mChannelNames.end(),
                         [search = QString(element.connName().c_str())](
                             auto const& name) { return name.name == search; });
  if (mChannelNames.end() != it) {
    int dst = static_cast<int>(std::distance(mChannelNames.begin(), it));
    emit dataChanged(createIndex(dst, 0), createIndex(dst, 0));
  }
}

void ChannelsListModel::removed(const ChangeListener::element& element) {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto it = std::find_if(mChannelNames.begin(), mChannelNames.end(),
                         [search = QString(element.connName().c_str())](
                             auto const& name) { return name.name == search; });
  if (mChannelNames.end() != it) {
    int dst = static_cast<int>(std::distance(mChannelNames.begin(), it));
    beginRemoveRows(QModelIndex(), dst, dst);
    mChannelNames.erase(it);
    endRemoveRows();
  }
}

void ChannelsListModel::updateChannelStatus(Channel::ChannelStatus status,
                                            const std::string& channelName,
                                            const std::string& message) {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto it = std::find_if(mChannelNames.begin(), mChannelNames.end(),
                         [search = QString(channelName.c_str())](
                             auto const& name) { return name.name == search; });
  if (mChannelNames.end() != it) {
    int dst = static_cast<int>(std::distance(mChannelNames.begin(), it));
    it->status = status;
    it->message = message.c_str();
    emit dataChanged(createIndex(dst, 0), createIndex(dst, 0));
  }
}
