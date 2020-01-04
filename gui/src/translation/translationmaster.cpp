#include "translationmaster.h"

#include "utils/applicationsettings.h"

#include <libintl.h>
#include <QApplication>
#include <QTranslator>
#include <clocale>

#include "spdlog/spdlog.h"

std::string getStrLocale(Language lang) {
  switch (lang) {
    case Language::EN:
      return "en_EN.UTF-8";
    case Language::RU:
      return "ru_RU.UTF-8";
  }
}

QString getFileName(Language lang) {
  switch (lang) {
    case Language::EN:
      return "Gui_en_EN";
    case Language::RU:
      return "Gui_ru_RU";
  }
}

TranslationMaster::TranslationMaster(
    std::shared_ptr<const ApplicationSettings> settings,
    const std::string& folder,
    QObject* parent) :
    QObject(parent),
    mSettings(std::move(settings)), mLang(mSettings->getLocale()),
    mFolder(folder) {
  setlocale(LC_ALL, getStrLocale(mLang).c_str());
  bindtextdomain("SecureDialogues", folder.c_str());
  bind_textdomain_codeset("SecureDialogues", "UTF-8");

  bindtextdomain("messages", folder.c_str());
  bind_textdomain_codeset("messages", "UTF-8");

  textdomain("SecureDialogues");

  if (!mTranslator.load(getFileName(mLang), mFolder.c_str())) {
    spdlog::get("root_logger")
        ->error("Cant install translator for lang {0} in path {1}",
                getFileName(mLang).toStdString(), mFolder);
  }

  QApplication::installTranslator(&mTranslator);
}

void TranslationMaster::onSettingsChanged() {
  if (const auto lang = mSettings->getLocale(); lang != mLang) {
    setlocale(LC_ALL, getStrLocale(mLang).c_str());
    mLang = lang;
    if (!mTranslator.load(getFileName(mLang), mFolder.c_str())) {
      spdlog::get("root_logger")
          ->error("Cant install translator for lang {0} in path {1}",
                  getFileName(mLang).toStdString(), mFolder);
    }

    QApplication::installTranslator(&mTranslator);

    emit langChanged();
  }
}
