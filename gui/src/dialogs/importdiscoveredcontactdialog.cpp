#include "importdiscoveredcontactdialog.h"
#include "ui_importdiscoveredcontactdialog.h"

#include "widgets/contactwidget.h"
#include "wrappers/recievedcontactsstoragewrapper.h"

#include <QMessageBox>

ImportDiscoveredContactDialog::ImportDiscoveredContactDialog(std::shared_ptr<ContactWidget> widget,
                                                             std::shared_ptr<RecievedContactsStorageWrapper> wrapper,
                                                             QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportDiscoveredContactDialog),
    mWidget(std::move(widget)),
    mWrapper(std::move(wrapper))
{
    ui->setupUi(this);
    ui->contactWidget->addWidget(mWidget.get());
    mWidget->actionEnable();
}

ImportDiscoveredContactDialog::~ImportDiscoveredContactDialog()
{
    delete ui;
}

void ImportDiscoveredContactDialog::onShowRecievedContact(std::string dialogId)
{
    mWidget->actionCleare();
    auto contact = mWrapper->getContact(dialogId);
    mWidget->setElement(contact);
    mDialogId = dialogId;
    this->show();
}

void ImportDiscoveredContactDialog::on_closeDialog_clicked()
{
   this->hide();
    mDialogId.reset();
}

void ImportDiscoveredContactDialog::on_importButton_clicked()
{
    try {
     const auto contact = mWidget->getElement();
     mWrapper->saveToContact(contact, mDialogId.value());
     mDialogId.reset();
     QMessageBox::information(this, "Импорт успешен", "Импорт контакта прошел успешно");
    } catch (const std::exception& ex) {
        QMessageBox::warning(this, "Ошибка импорта!", ex.what());
    }
}
