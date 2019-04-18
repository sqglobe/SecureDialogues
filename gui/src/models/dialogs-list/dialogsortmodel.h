#ifndef DIALOGSORTMODEL_H
#define DIALOGSORTMODEL_H

#include <QSortFilterProxyModel>

class DialogSortModel : public QSortFilterProxyModel {
 public:
  using QSortFilterProxyModel::QSortFilterProxyModel;

 protected:
  bool lessThan(const QModelIndex& source_left,
                const QModelIndex& source_right) const override;
};

#endif  // DIALOGSORTMODEL_H
