#include "appsettingsdialog.h"
#include "ui_appsettingsdialog.h"

#include "applicationsettings.h"
#include <QMessageBox>

AppSettingsDialog::AppSettingsDialog(const std::shared_ptr<ApplicationSettingsGuard> &guard,
                                     QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppSettingsDialog),
    mSettings(guard->getEditableSettings())
{
    ui->setupUi(this);
    ui->languageSettings->addItem("English", "en_EN.utf8");
    ui->languageSettings->addItem("Русский", "ru_RU.utf8");
    const auto lang = mSettings->getLocale();

    if(const auto index = ui->languageSettings->findData(lang); index != -1)
    {
        ui->languageSettings->setCurrentIndex(index);
    }

}

AppSettingsDialog::~AppSettingsDialog()
{
    delete ui;
}

void AppSettingsDialog::on_saveButton_clicked()
{
   const auto lang = ui->languageSettings->currentData().value<QString>();
   mSettings->setLocale(lang);

   QMessageBox::information(this, "Успешно!", "Настройки успешно сохранены");

   this->hide();

   emit settingsChanged();
}
