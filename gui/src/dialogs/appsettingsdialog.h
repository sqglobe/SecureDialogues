#ifndef APPSETTINGSDIALOG_H
#define APPSETTINGSDIALOG_H

#include <QDialog>
#include <memory>

#include "ui_appsettingsdialog.h"
#include "utils/translatechangeeventhandler.h"

class ApplicationSettingsGuard;
class ApplicationSettings;

class AppSettingsDialog
    : public TranslateChangeEventHandler<QDialog, Ui::AppSettingsDialog> {
  Q_OBJECT

 public:
  explicit AppSettingsDialog(
      const std::shared_ptr<ApplicationSettingsGuard>& guard,
      QWidget* parent = nullptr);
  ~AppSettingsDialog() override;

 signals:
  void settingsChanged();

 private slots:
  void on_saveButton_clicked();

 private:
  Ui::AppSettingsDialog* ui;

 private:
  std::shared_ptr<ApplicationSettings> mSettings;
};

#endif  // APPSETTINGSDIALOG_H
