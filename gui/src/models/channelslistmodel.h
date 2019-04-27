#ifndef CHANNELSLISTMODEL_H
#define CHANNELSLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QVector>
#include <mutex>
#include <vector>

#include "interfaces/changewatcher.h"

class ConnectionHolder;

/**
 * с Модель отображает список существующих подключений.
 * Используется в диалоге управления контактами для выбора подключения,
 * через которое будет осуществляться общение с пользователем
 */
class ChannelsListModel : public QAbstractListModel,
                          public ChangeWatcher<ConnectionHolder> {
 public:
  explicit ChannelsListModel(const std::vector<ConnectionHolder>& elements);

 public:
  int rowCount(const QModelIndex& = QModelIndex()) const override;
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;

 public:
  virtual void added(const element& obj) override;
  virtual void changed(const element&) override;
  virtual void removed(const element& obj) override;

 private:
  QVector<QString> mChannelNames;
  mutable std::recursive_mutex mMutex;
};

#endif  // CHANNELSLISTMODEL_H
