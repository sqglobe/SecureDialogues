#ifndef DISCOVEREDCONTACTMODEL_H
#define DISCOVEREDCONTACTMODEL_H

#include <QAbstractListModel>
#include <mutex>
#include <vector>
#include "containers/storages.h"

class DiscoveredContactModel : public QAbstractListModel,
                               public ChangeListener<DiscoveredContact> {
 public:
  constexpr static int ContactRole = 0x0101;

 public:
  DiscoveredContactModel(std::vector<DiscoveredContact> data,
                         QObject* parent = nullptr);

 public:
  int rowCount(const QModelIndex& = QModelIndex()) const override;
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex&) const override;

 public:
  virtual void added(const element& obj) override;
  virtual void changed(const element& obj) override;
  virtual void removed(const element& obj) override;

 private:
  std::vector<DiscoveredContact> mData;
  mutable std::recursive_mutex mMutex;
};

#endif  // DISCOVEREDCONTACTMODEL_H
