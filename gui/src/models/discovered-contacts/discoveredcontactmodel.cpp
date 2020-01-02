#include "discoveredcontactmodel.h"
#include <QApplication>
#include <ctime>

Q_DECLARE_METATYPE(std::string);

QString convert(const DiscoveredContact& contact) {
  static const QString arg(
      QApplication::tr("Recieved from: %1, date: %2\nAddress: %3"));
  std::time_t now_c = std::chrono::system_clock::to_time_t(contact.created());
  std::string data{50, '\0'};
  std::strftime(&data[0], 50, "%d %m %Y %H:%M:%S", std::localtime(&now_c));
  return arg.arg(
      QString::fromUtf8(contact.name().data(),
                        static_cast<int>(contact.name().length())),
      QString::fromUtf8(data.c_str()),
      QString::fromUtf8(contact.contact_adress().data(),
                        static_cast<int>(contact.contact_adress().length())));
}

DiscoveredContactModel::DiscoveredContactModel(
    std::vector<DiscoveredContact> data,
    QObject* parent) :
    QAbstractListModel(std::move(parent)),
    mData(data) {
  std::cout << 1 << std::endl;
}

int DiscoveredContactModel::rowCount(const QModelIndex&) const {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> guard(mMutex);
  return static_cast<int>(mData.size());
}

QVariant DiscoveredContactModel::data(const QModelIndex& index,
                                      int role) const {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> guard(mMutex);
  if (index.row() < 0 || index.row() >= static_cast<int>(mData.size())) {
    return QVariant();
  } else if (role == Qt::DisplayRole) {
    const auto p = static_cast<std::size_t>(index.row());
    return QVariant::fromValue(convert(mData[p]));
  } else if (role == ContactRole) {
    const auto p = static_cast<std::size_t>(index.row());
    return QVariant::fromValue(std::string(mData[p].dialog_id()));
  }
  return QVariant();
}

Qt::ItemFlags DiscoveredContactModel::flags(const QModelIndex&) const {
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void DiscoveredContactModel::added(const ChangeListener::element& obj) {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> guard(mMutex);
  const auto newPos = static_cast<int>(mData.size());
  beginInsertRows(QModelIndex(), newPos, newPos);
  mData.emplace_back(obj);
  endInsertRows();
}

void DiscoveredContactModel::changed(const ChangeListener::element& obj) {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto it = std::find_if(mData.begin(), mData.end(),
                         [id = obj.dialog_id()](const auto& elem) {
                           return id == elem.dialog_id();
                         });
  if (it == mData.end()) {
    return;
  }
  *it = obj;
  auto newPos = static_cast<int>(std::distance(mData.begin(), it));
  emit dataChanged(createIndex(newPos, 0), createIndex(newPos, 0));
}

void DiscoveredContactModel::removed(const ChangeListener::element& obj) {
  [[maybe_unused]] std::lock_guard<std::recursive_mutex> guard(mMutex);
  auto it = std::find_if(mData.begin(), mData.end(),
                         [id = obj.dialog_id()](const auto& elem) {
                           return id == elem.dialog_id();
                         });
  if (it == mData.end()) {
    return;
  }

  beginResetModel();
  mData.erase(it);
  endResetModel();
}
