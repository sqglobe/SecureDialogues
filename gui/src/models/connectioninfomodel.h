#ifndef CONNECTIONINFOMODEL_H
#define CONNECTIONINFOMODEL_H

#include <memory>
#include <mutex>
#include <vector>
#include "communication/channel.h"
#include "containermodelgenerator.h"
#include "identifiedlistmodel.h"
#include "interfaces/changelistener.h"
/**
 * @brief Сгенерированный класс, который отображает список подключений.
 * Используется в диалоге управления подключениями
 */

class ConnectionHolder;

class ConnectionInfoModel : public IdentifiedListModel,
                            public ChangeListener<ConnectionHolder> {
 public:
  explicit ConnectionInfoModel(const std::vector<element>& elements);

 public:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;

 public:
  std::optional<std::string> getId(const QModelIndex& index) const override;

 public:
  void added(const element& element) override;
  void changed(const element&) override;
  void removed(const element& element) override;

 public:
  void updateChannelStatus(Channel::ChannelStatus status,
                           const std::string& channelName,
                           const std::string&);

 private:
  inline int simpleRowCount() const;

 private:
  struct ConnData {
    QString connName;
    Channel::ChannelStatus status;
  };

 private:
  std::vector<ConnData> mData;
  mutable std::recursive_mutex mMutex;
};

int ConnectionInfoModel::simpleRowCount() const {
  return static_cast<int>(mData.size());
}

#endif  // CONNECTIONINFOMODEL_H
