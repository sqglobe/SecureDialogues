#ifndef IDENTIFIEDLISTMODEL_H
#define IDENTIFIEDLISTMODEL_H
#include <QAbstractListModel>
#include <optional>

class IdentifiedListModel : public QAbstractListModel {
 public:
  virtual std::optional<std::string> getId(const QModelIndex& index) const = 0;
};

#endif  // IDENTIFIEDLISTMODEL_H
