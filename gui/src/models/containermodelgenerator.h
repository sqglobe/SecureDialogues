#ifndef CONTAINERMODELGENERATOR_H
#define CONTAINERMODELGENERATOR_H

#include <QAbstractListModel>
#include <memory>
#include "interfaces/changewatcher.h"

/**
 * Макрос для генерации класса-модели. Используется для инкапсуляции
 * повторяющегося кода. В этом случае использование шаблонов не применимо, так
 * как система сигналов-слотов, необходимая для работы модели не функционирует с
 * шаблонами.
 *
 * Needed to create function
 * to_qstring(const ContainerClass::const_element &)
 */

#define GENERATE_CONTAINER_MODEL(ModelClass, ContainerClass)                   \
  class ModelClass : public QAbstractListModel,                                \
                     public ChangeWatcher<ContainerClass::const_element> {     \
   public:                                                                     \
    ModelClass(const std::shared_ptr<ContainerClass>& cont);                   \
    ContainerClass::const_element getAt(const QModelIndex& pos) const;         \
                                                                               \
   public:                                                                     \
    virtual int rowCount(                                                      \
        const QModelIndex& parent = QModelIndex()) const override;             \
    virtual QVariant data(const QModelIndex& index,                            \
                          int role = Qt::DisplayRole) const override;          \
                                                                               \
   public:                                                                     \
    virtual void added(const element& obj) override;                           \
    virtual void changed(const element& obj) override;                         \
    virtual void removed(const element& obj) override;                         \
                                                                               \
   private:                                                                    \
    std::shared_ptr<ContainerClass> mContainer;                                \
  };                                                                           \
                                                                               \
  inline ModelClass::ModelClass(const std::shared_ptr<ContainerClass>& cont) : \
      mContainer(cont) {}                                                      \
                                                                               \
  inline int ModelClass::rowCount([[maybe_unused]] const QModelIndex& parent)  \
      const {                                                                  \
    return mContainer->size();                                                 \
  }                                                                            \
                                                                               \
  inline QVariant ModelClass::data(const QModelIndex& index, int role) const { \
    if (Qt::DisplayRole == role && index.row() >= 0 &&                         \
        index.row() < rowCount()) {                                            \
      return to_qstring(mContainer->at(index.row()));                          \
    }                                                                          \
                                                                               \
    return QVariant();                                                         \
  }                                                                            \
                                                                               \
  inline void ModelClass::added(const ChangeWatcher::element& obj) {           \
    auto newPos = mContainer->posOf(get_id(obj));                              \
    beginInsertRows(QModelIndex(), newPos, newPos);                            \
    endInsertRows();                                                           \
  }                                                                            \
                                                                               \
  inline void ModelClass::changed(const ChangeWatcher::element& obj) {         \
    auto newPos = mContainer->posOf(get_id(obj));                              \
    emit dataChanged(createIndex(newPos, 0), createIndex(newPos, 0));          \
  }                                                                            \
                                                                               \
  inline void ModelClass::removed(                                             \
      [[maybe_unused]] const ChangeWatcher::element& obj) {                    \
    beginResetModel();                                                         \
    endResetModel();                                                           \
  }                                                                            \
  inline ContainerClass::const_element ModelClass::getAt(                      \
      const QModelIndex& pos) const {                                          \
    return mContainer->at(pos.row());                                          \
  }

#endif  // CONTAINERMODELGENERATOR_H
