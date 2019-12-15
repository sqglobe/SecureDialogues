#include "recieveddiscoveredcontactsdialog.h"
#include "ui_recieveddiscoveredcontactsdialog.h"

#include <QMessageBox>
#include "models/discovered-contacts/discoveredcontactmodel.h"
#include "wrappers/recievedcontactsstoragewrapper.h"

Q_DECLARE_METATYPE(std::string);

RecievedDiscoveredContactsDialog::RecievedDiscoveredContactsDialog(
    DiscoveredContactModel* model,
    std::shared_ptr<RecievedContactsStorageWrapper> storageWrapper,
    QWidget* parent) :
    QDialog(parent),
    ui(new Ui::RecievedDiscoveredContactsDialog),
    mStorageWrapper(std::move(storageWrapper)) {
  ui->setupUi(this);

  ui->recievedContacts->setModel(model);
  ui->recievedContacts->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->recievedContacts, &QListView::customContextMenuRequested, this,
          &RecievedDiscoveredContactsDialog::requestToShowMenu);
}

RecievedDiscoveredContactsDialog::~RecievedDiscoveredContactsDialog() {
  delete ui;
}

void RecievedDiscoveredContactsDialog::requestToShowMenu(QPoint pos) {
  const auto index = ui->recievedContacts->indexAt(pos);
  if (!index.isValid()) {
    return;
  }
  const auto data = ui->recievedContacts->model()->data(
      index, DiscoveredContactModel::ContactRole);
  if (!data.isValid() || !data.canConvert<std::string>()) {
    return;
  }
  const auto dialogId = qvariant_cast<std::string>(data);
  emit showDialogMenu(ui->recievedContacts->viewport()->mapToGlobal(pos),
                      dialogId);
}

void RecievedDiscoveredContactsDialog::removeRecievedContact(
    std::string dialogId) {
  const auto res = QMessageBox::question(
      this, "Удаление полученных контактных данных",
      "Вы действительно хотите удалить полученные контактные данные?",
      QMessageBox::Apply | QMessageBox::Cancel);
  if (QMessageBox::Apply == res) {
    if (mStorageWrapper->removeDiscoveredContact(dialogId)) {
      QMessageBox::information(this, "Успешно!",
                               "Данные были успешно удалены!");
    } else {
      QMessageBox::warning(
          this, "Ошибка!",
          "При удалении возникла ошибка, пожалуйста попытайтесь позже!");
    }
  }
}
