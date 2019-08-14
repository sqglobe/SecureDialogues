#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <mutex>
#include <vector>
#include "identifiedlistmodel.h"
#include "interfaces/changelistener.h"
#include "primitives/contact.h"
/**
 * @brief Сгенерированный класс, который отображает список контактов.
 * Используется в диалоге управления контактами
 */
class ContactModel : public IdentifiedListModel,
                     public ChangeListener<Contact> {
 public:
  explicit ContactModel(const std::vector<Contact>& contacts);

 public:
  virtual int rowCount(const QModelIndex& = QModelIndex()) const override;
  virtual QVariant data(const QModelIndex& index,
                        int role = Qt::DisplayRole) const override;

 public:
  std::optional<std::string> getId(const QModelIndex& index) const override;

 public:
  void added(const element& element) override;
  void changed(const element& element) override;
  void removed(const element& element) override;

 private:
  inline int size() const;

 private:
  struct Element {
    std::string id;
    QString display;
  };

 private:
  mutable std::mutex mMutex;
  std::vector<Element> mData;
};

int ContactModel::size() const {
  return static_cast<int>(mData.size());
}

#endif  // CONTACTMODEL_H
