#include "contactmodel.h"
#include "gui_helpers.h"

static QString format("%1 (%2)");

ContactModel::ContactModel(const std::vector<Contact>& contacts) {
  std::transform(std::cbegin(contacts), std::cend(contacts),
                 std::back_inserter(mData), [](const auto& contact) -> Element {
                   return {std::string(contact.id()),
                           format.arg(make_qstring(contact.name()),
                                      make_qstring(contact.adress()))};
                 });
}

int ContactModel::rowCount(const QModelIndex&) const {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> lock(mMutex);
  return size();
}

QVariant ContactModel::data(const QModelIndex& index, int role) const {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> lock(mMutex);

  if (!index.isValid())
    return QVariant();

  if (Qt::DisplayRole == role) {
    return mData.at(static_cast<std::size_t>(index.row())).display;
  }
  return QVariant();
}

std::optional<std::string> ContactModel::getId(const QModelIndex& index) const {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> lock(mMutex);
  if (index.isValid()) {
    return mData.at(static_cast<std::size_t>(index.row())).id;
  }
  return {};
}

void ContactModel::added(const ChangeListener::element& element) {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> lock(mMutex);
  beginInsertRows(QModelIndex(), size(), size());
  mData.push_back(
      {std::string(element.id()), format.arg(make_qstring(element.name()),
                                             make_qstring(element.adress()))});
  endInsertRows();
}

void ContactModel::changed(const ChangeListener::element& element) {
  auto iter =
      std::find_if(std::begin(mData), std::end(mData),
                   [id = element.id()](auto const& el) { return id == el.id; });
  if (iter != std::end(mData)) {
    auto diff = static_cast<int>(std::distance(std::begin(mData), iter));
    iter->display = format.arg(make_qstring(element.name()),
                               make_qstring(element.adress()));
    emit dataChanged(createIndex(diff, 0), createIndex(diff, 0));
  }
}

void ContactModel::removed(const ChangeListener::element& element) {
  auto it =
      std::find_if(std::begin(mData), std::end(mData),
                   [id = element.id()](const auto& el) { return el.id == id; });
  if (it != std::end(mData)) {
    auto diff = static_cast<int>(std::distance(std::begin(mData), it));
    beginRemoveRows(QModelIndex(), diff, diff);
    mData.erase(it);
    endRemoveRows();
  }
}
