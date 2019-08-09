#ifndef CONNECTIONINFOMODEL_H
#define CONNECTIONINFOMODEL_H

#include <persistent-storage/watchers/enqueuedevents.h>
#include <QAbstractListModel>
#include <memory>
#include <vector>
#include "communication/channel.h"
#include "containermodelgenerator.h"
/**
 * @brief Сгенерированный класс, который отображает список подключений.
 * Используется в диалоге управления подключениями
 */

class ConnectionHolder;

class ConnectionInfoModel : public QAbstractListModel {
 public:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;

 public:
  void operator()(prstorage::EnqueuedEvents event,
                  const ConnectionHolder& holder);

 private:
  struct ConnData {
    QString connName;
    Channel::ChannelStatus status;
  };

 private:
  std::vector<ConnData> mData;
};

#endif  // CONNECTIONINFOMODEL_H
