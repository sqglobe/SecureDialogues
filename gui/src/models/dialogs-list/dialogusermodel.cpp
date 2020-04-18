#include "dialogusermodel.h"
#include "containers/storages.h"

#include <QColor>
#include <QString>

Q_DECLARE_METATYPE(DialogInfo)

DialogUserModel::DialogUserModel(std::shared_ptr<ContactStorage> contacts,
                                 const std::vector<Dialog>& dialogs) :
    mContacts(std::move(contacts)) {
  std::transform(std::cbegin(dialogs), std::cend(dialogs),
                 std::back_inserter(mDialogsInfo),
                 [&contacts](const Dialog& dialog) -> DialogInfo {
                   auto contact =
                       contacts->get(std::string(dialog.getContactId()));
                   return DialogInfo(dialog, contact);
                 });
}

int DialogUserModel::rowCount(const QModelIndex&) const {
  return static_cast<int>(mDialogsInfo.size());
}

QVariant DialogUserModel::data(const QModelIndex& index, int role) const {
  if (index.row() < 0 || index.row() >= rowCount()) {
    return QVariant();
  } else if (Qt::DisplayRole == role) {
    auto p = static_cast<std::size_t>(index.row());
    return QVariant::fromValue(mDialogsInfo[p]);
  }
  return QVariant();
}

void DialogUserModel::added(const Dialog& obj) {
  auto newPos = static_cast<int>(mDialogsInfo.size());
  auto contact = mContacts->get(std::string(obj.getContactId()));
  beginInsertRows(QModelIndex(), newPos, newPos);
  mDialogsInfo.emplace_back(obj, contact);
  endInsertRows();

  if (rowCount() == 1) {
    updateActiveDialog(DialogInfo(obj, contact));
  }
}

void DialogUserModel::changed(const Dialog& obj) {
  auto it = std::find_if(mDialogsInfo.begin(), mDialogsInfo.end(),
                         [id = obj.getDialogId()](const auto& elem) {
                           return id == elem.dialogId();
                         });
  if (it == mDialogsInfo.end()) {
    return;
  }
  *it = obj;
  auto newPos = static_cast<int>(std::distance(mDialogsInfo.begin(), it));
  emit dataChanged(createIndex(newPos, 0), createIndex(newPos, 0));
  if (activeDialog == obj.getDialogId()) {
    updateActiveDialog(*it);
    it->messagesReaded();
  }
}

void DialogUserModel::removed(const Dialog& obj) {
  auto it = std::find_if(mDialogsInfo.cbegin(), mDialogsInfo.cend(),
                         [id = obj.getDialogId()](const auto& elem) {
                           return id == elem.dialogId();
                         });
  if (it == mDialogsInfo.cend()) {
    return;
  }

  beginResetModel();
  mDialogsInfo.erase(it);
  endResetModel();

  if (activeDialog == obj.getDialogId()) {
    if (mDialogsInfo.empty()) {
      activeDialog = "";
      emit notValidDialogWasSelected(activeDialog);
    } else {
      updateActiveDialog(mDialogsInfo[0]);
      mDialogsInfo[0].messagesReaded();
    }
  }
}

void DialogUserModel::added(const Contact&) {}

void DialogUserModel::changed(const Contact& obj) {
  beginResetModel();
  for (auto& dialogInfo : mDialogsInfo) {
    if (dialogInfo.contactId() == obj.id()) {
      dialogInfo = obj;
    }
  }
  endResetModel();
}

void DialogUserModel::removed(const Contact&) {}

void DialogUserModel::messageAdded(const std::string& dialogId,
                                   const std::shared_ptr<const UserMessage>&) {
  if (activeDialog == dialogId) {
    return;
  }

  auto it = std::find_if(
      mDialogsInfo.begin(), mDialogsInfo.end(),
      [&dialogId](const auto& elem) { return dialogId == elem.dialogId(); });

  if (it == mDialogsInfo.end()) {
    return;
  }

  it->addUnreadMessage();

  auto newPos = static_cast<int>(std::distance(mDialogsInfo.begin(), it));
  emit dataChanged(createIndex(newPos, 0), createIndex(newPos, 0));
}

void DialogUserModel::activeDialogChanged(const std::string&) {}

void DialogUserModel::peekMessage(const std::shared_ptr<const UserMessage>&) {}

void DialogUserModel::invalidateData(const std::string&) {}

std::pair<Dialog::Status, std::string> DialogUserModel::getDialogInfo(
    const QModelIndex& pos) const {
  auto i = static_cast<std::size_t>(pos.row());
  return std::make_pair(mDialogsInfo[i].status(), mDialogsInfo[i].dialogId());
}

void DialogUserModel::updateActiveDialog(const DialogInfo& obj) {
  activeDialog = obj.dialogId();
  if (Dialog::Status::ACTIVE == obj.status()) {
    emit newDialogWasSelected(activeDialog);
  } else {
    emit notValidDialogWasSelected(activeDialog);
  }
}

void DialogUserModel::rowWasSelected(const QModelIndex& index) {
  if (index.row() < 0 || index.row() >= rowCount()) {
    return;
  }
  mDialogsInfo[static_cast<std::size_t>(index.row())].messagesReaded();
  updateActiveDialog(mDialogsInfo[static_cast<std::size_t>(index.row())]);
}
