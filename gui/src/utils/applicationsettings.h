#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QSettings>
#include <memory>
#include "export/languages.h"
class ApplicationSettings {
 public:
  ApplicationSettings();

 public:
  Language getLocale() const;

 public:
  void setLocale(Language locale);

 private:
  QSettings mSettings;
};

class ApplicationSettingsGuard {
  friend class AppSettingsDialog;

 public:
  ApplicationSettingsGuard() :
      mSettings(std::make_shared<ApplicationSettings>()) {}

 public:
  std::shared_ptr<const ApplicationSettings> getSettings() const {
    return mSettings;
  }

 private:
  std::shared_ptr<ApplicationSettings> getEditableSettings() const {
    return mSettings;
  }

 private:
  std::shared_ptr<ApplicationSettings> mSettings;
};

#endif  // APPLICATIONSETTINGS_H
