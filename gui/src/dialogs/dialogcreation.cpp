#include "dialogcreation.h"
#include "ui_dialogcreation.h"

#include "models/contactmodel.h"

#include <QMessageBox>
#include <QSortFilterProxyModel>

DialogCreation::DialogCreation(const std::shared_ptr<ContactModel>& model,
                               QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DialogCreation), mModel(model),
    mProxy(new QSortFilterProxyModel(this)) {
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
    QMessageBox::critical(this, "Внимание!",
                          "Для продолжения необходимо выбрать один контакт");
  } else {
    try {
      auto sIndex = mProxy->mapToSource(index);
      auto contact = mModel->getAt(sIndex);
      auto res = QMessageBox::question(
          this, "Необходим выбор",
          QString("Вы действительно хотите начать диалог с ")
              .append(to_qstring(contact)),
          QMessageBox::Apply | QMessageBox::Cancel);
      if (QMessageBox::Apply == res) {
        emit createNewDialog(contact);
        this->hide();
      }

    } catch (std::exception& ex) {
      QMessageBox::critical(
          this, "Внимание!",
          QString("При создании нового диалога произошла ошибка: ") +
              ex.what());
    }
  }
}

void DialogCreation::on_buttonBox_rejected() {
  this->hide();
}
