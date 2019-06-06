#ifndef CHANNELSLISTMODEL_H
#define CHANNELSLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QVector>
#include <mutex>
#include <vector>

#include "communication/channel.h"
#include "interfaces/changewatcher.h"
#include "primitives/connectionholder.h"

/**
 * с Модель отображает список существующих подключений.
 * Используется в диалоге управления контактами для выбора подключения,
 * через которое будет осуществляться общение с пользователем
 */
class ChannelsListModel : public QAbstractListModel,
                          public ChangeWatcher<ConnectionHolder> {
 public:
  struct ListItem {
    QString name;
    QString message;
    Channel::ChannelStatus status;
    ConnectionType connectionType;
  };

 public:
  explicit ChannelsListModel(const std::vector<ConnectionHolder>& elements);

 public:
  int rowCount(const QModelIndex& = QModelIndex()) const override;
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;

 public:
  void added(const element& obj) override;
  void changed(const element& obj) override;
  void removed(const element& obj) override;

 private:
  QVector<ListItem> mChannelNames;
  mutable std::recursive_mutex mMutex;
};

#endif  // CHANNELSLISTMODEL_H
