#include "translationmaster.h"

#include "utils/applicationsettings.h"

#include <libintl.h>
#include <QApplication>
#include <QTranslator>
#include <clocale>

#include "plugininterface.h"
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
    std::shared_ptr<const plugin_support::PluginsContainer> plugins,
    QObject* parent) :
    QObject(parent),
    mSettings(std::move(settings)), mLang(mSettings->getLocale()),
    mFolder(folder), mPlugins(std::move(plugins)) {
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

  auto deleter = [](QTranslator* translator) {
    QApplication::removeTranslator(translator);
    delete translator;
  };

  std::for_each(
      mPlugins->cbegin(), mPlugins->cend(),
      [this, deleter](std::shared_ptr<plugin_support::PluginInterface> plugin) {
        std::string domain = plugin->getGettextDomain();
        bindtextdomain(domain.c_str(), plugin->getLocaleFolder().c_str());
        bind_textdomain_codeset(domain.c_str(), "UTF-8");
        TranslateUnit unit = {
            std::unique_ptr<QTranslator, std::function<void(QTranslator*)>>(
                new QTranslator, deleter),
            plugin};
        if (unit.translator->load(plugin->getTranslationFileName(mLang).c_str(),
                                  plugin->getLocaleFolder().c_str())) {
          QApplication::installTranslator(unit.translator.get());
          mPluginTranslators.push_back(std::move(unit));
        } else {
          spdlog::get("root_logger")
              ->error("Cant install translator for lang {0} in path {1}",
                      plugin->getTranslationFileName(mLang).c_str(),
                      plugin->getLocaleFolder());
        }
      });

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

    std::for_each(
        mPluginTranslators.cbegin(), mPluginTranslators.cend(),
        [this](const TranslateUnit& unit) {
          if (!unit.translator->load(
                  unit.interface->getTranslationFileName(mLang).c_str(),
                  unit.interface->getLocaleFolder().c_str())) {
            spdlog::get("root_logger")
                ->error("Cant install translator for lang {0} in path {1}",
                        unit.interface->getTranslationFileName(mLang).c_str(),
                        unit.interface->getLocaleFolder().c_str());
          }
        });

    QApplication::installTranslator(&mTranslator);

    emit langChanged();
  }
}
