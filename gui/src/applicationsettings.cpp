#include "applicationsettings.h"

ApplicationSettings::ApplicationSettings() :
    mSettings("secure-dialogues", "app") {}

Language ApplicationSettings::getLocale() const {
  if (!mSettings.contains("language")) {
    return Language::EN;
  }

  auto value = mSettings.value("language");
  if (value.canConvert<int>()) {
    return static_cast<Language>(value.value<int>());
  }
  return Language::EN;
}

void ApplicationSettings::setLocale(Language locale) {
  mSettings.setValue("language", QVariant::fromValue(static_cast<int>(locale)));
  mSettings.sync();
}
