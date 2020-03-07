#ifndef CHANNELSLISTMODEL_H
#define CHANNELSLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QVector>
#include <mutex>
#include <vector>

#include "communication/channel.h"
#include "primitives/connectionholder.h"

#include "interfaces/changelistener.h"
#include "pluginscontainer.h"

class PluginAddressValidator;
/**
 * с Модель отображает список существующих подключений.
 * Используется в диалоге управления контактами для выбора подключения,
 * через которое будет осуществляться общение с пользователем
 */
class ChannelsListModel : public QAbstractListModel,
                          public ChangeListener<ConnectionHolder> {
 public:
  struct ListItem {
    QString name;
    QString message;
    Channel::ChannelStatus status;
    std::shared_ptr<const PluginAddressValidator> validator;
  };

 public:
  explicit ChannelsListModel(
      const std::vector<ConnectionHolder>& elements,
      std::shared_ptr<const plugin_support::PluginsContainer> container);

 public:
  int rowCount(const QModelIndex& = QModelIndex()) const override;
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;

 public:
  void added(const element& element) override;
  void changed(const element& element) override;
  void removed(const element& element) override;

 public:
  void updateChannelStatus(Channel::ChannelStatus status,
                           const std::string& channelName,
                           const std::string& message);

 private:
  QVector<ListItem> mChannelNames;
  mutable std::recursive_mutex mMutex;
  std::shared_ptr<const plugin_support::PluginsContainer> mContainer;
};

#endif  // CHANNELSLISTMODEL_H
