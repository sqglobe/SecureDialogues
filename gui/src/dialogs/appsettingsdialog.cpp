#include "appsettingsdialog.h"
#include "ui_appsettingsdialog.h"

#include <QMessageBox>
#include "applicationsettings.h"

AppSettingsDialog::AppSettingsDialog(
    const std::shared_ptr<ApplicationSettingsGuard>& guard,
    QWidget* parent) :
    QDialog(parent),
    ui(new Ui::AppSettingsDialog), mSettings(guard->getEditableSettings()) {
  ui->setupUi(this);
  ui->languageSettings->addItem(
      "English", QVariant::fromValue(static_cast<int>(Language::EN)));
  ui->languageSettings->addItem(
      "Русский", QVariant::fromValue(static_cast<int>(Language::RU)));
  const auto lang = static_cast<int>(mSettings->getLocale());

  if (const auto index =
          ui->languageSettings->findData(QVariant::fromValue(lang));
      index != -1) {
    ui->languageSettings->setCurrentIndex(index);
  }
}

AppSettingsDialog::~AppSettingsDialog() {
  delete ui;
}

void AppSettingsDialog::changeEvent(QEvent* event) {
  if (event->type() == QEvent::LanguageChange) {
    ui->retranslateUi(this);  // переведём окно заново
  } else {
    QDialog::changeEvent(event);
  }
}

void AppSettingsDialog::on_saveButton_clicked() {
  const auto lang =
      static_cast<Language>(ui->languageSettings->currentData().value<int>());
  mSettings->setLocale(lang);

  QMessageBox::information(this, tr("Success!"),
                           tr("Settings saved successful"));

  this->hide();

  emit settingsChanged();
}
