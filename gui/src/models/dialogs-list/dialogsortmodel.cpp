#include "dialogsortmodel.h"
#include "dialoginfo.h"

Q_DECLARE_METATYPE(DialogInfo)

int getRating(Dialog::Status status) {
  using S = Dialog::Status;
  if (status == S::ACTIVE)
    return 1;
  else if (status == S::NEW || status == S::CREATE_REQUEST)
    return 2;
  else if (status == S::WAIT_CONFIRM || status == S::ACCEPTED_WAIT ||
           status == S::WAIT_KEY_VERIFICAION)
    return 3;
  else if (status == S::CLOSED || status == S::ABORTED)
    return 4;
  return 10;
}

bool DialogSortModel::lessThan(const QModelIndex& source_left,
                               const QModelIndex& source_right) const {
  auto left_data = sourceModel()->data(source_left);
  auto right_data = sourceModel()->data(source_right);
  if (!left_data.canConvert<DialogInfo>() ||
      !right_data.canConvert<DialogInfo>()) {
    return QSortFilterProxyModel::lessThan(source_left, source_right);
  }

  const auto& left = qvariant_cast<DialogInfo>(left_data);
  const auto& right = qvariant_cast<DialogInfo>(right_data);

  auto leftRating = getRating(left.status());
  auto rightRating = getRating(right.status());

  if (leftRating == rightRating) {
    return left.lastUpdated() < right.lastUpdated();
  }

  return leftRating > rightRating;
}
