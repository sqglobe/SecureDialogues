#ifndef CONNECTIONINFOMODEL_H
#define CONNECTIONINFOMODEL_H

#include <QAbstractListModel>
#include <memory>
#include "containermodelgenerator.h"
#include "containers/connectioninfocontainer.h"
#include "interfaces/changewatcher.h"
/**
 * @brief Сгенерированный класс, который отображает список подключений.
 * Используется в диалоге управления подключениями
 */

class ConnectionInfoModel
    : public QAbstractListModel,
      public ChangeWatcher<ConnectionInfoContainer::const_element> {
 public:
  explicit ConnectionInfoModel(
      const std::shared_ptr<ConnectionInfoContainer>& cont);
  ConnectionInfoContainer::const_element getAt(const QModelIndex& pos) const;

 public:
  virtual int rowCount(
      const QModelIndex& parent = QModelIndex()) const override;
  virtual QVariant data(const QModelIndex& index,
                        int role = Qt::DisplayRole) const override;

 public:
  virtual void added(const element& obj) override;
  virtual void changed(const element& obj) override;
  virtual void removed(const element& obj) override;

 private:
  std::shared_ptr<ConnectionInfoContainer> mContainer;
};

#endif  // CONNECTIONINFOMODEL_H
