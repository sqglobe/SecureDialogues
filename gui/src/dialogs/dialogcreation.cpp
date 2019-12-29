#include "dialogcreation.h"
#include "ui_dialogcreation.h"

#include "models/contactmodel.h"

#include <QMessageBox>
#include <QSortFilterProxyModel>

#include "gui_helpers.h"

DialogCreation::DialogCreation(std::shared_ptr<ContactModel> model,
                               std::shared_ptr<ContactStorage> storage,
                               QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DialogCreation), mModel(std::move(model)),
    mProxy(new QSortFilterProxyModel(this)), mStorage(std::move(storage)) {
  ui->setupUi(this);
  mProxy->setSourceModel(mModel.get());
  ui->contactsView->setModel(mProxy);
}

DialogCreation::~DialogCreation() {
  delete ui;
}

void DialogCreation::on_buttonBox_accepted() {
  auto index = ui->contactsView->currentIndex();
  if (!index.isValid()) {
    QMessageBox::critical(this, tr("Error!"),
                          tr("Your have to select one of the connections to share your contact info"));
  } else {
    try {
      auto sIndex = mProxy->mapToSource(index);
      auto id = mModel->getId(sIndex);

      if (!id)
        return;

      auto contact = mStorage->get(id.value());
      auto res = QMessageBox::question(
          this, tr("Choice required"),
         tr("Do you want to start conversation with ")
              .append(make_qstring(contact.name()))
              .append("(")
              .append(make_qstring(contact.adress()))
              .append(")"),
          QMessageBox::Apply | QMessageBox::Cancel);
      if (QMessageBox::Apply == res) {
        emit createNewDialog(contact);
        this->hide();
      }

    } catch (std::exception& ex) {
      QMessageBox::critical(
          this, tr("Error!"),
          tr("Error occured during dialog creation: ") +
              ex.what());
    }
  }
}

void DialogCreation::on_buttonBox_rejected() {
  this->hide();
}
