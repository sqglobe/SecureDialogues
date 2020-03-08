#ifndef TRANSLATIONMASTER_H
#define TRANSLATIONMASTER_H

#include <QObject>
#include <QTranslator>
#include <functional>
#include <list>
#include <memory>
#include <string>
#include "export/languages.h"
#include "pluginscontainer.h"

class ApplicationSettings;

class TranslationMaster : public QObject {
  Q_OBJECT

 private:
  struct TranslateUnit {
    std::unique_ptr<QTranslator, std::function<void(QTranslator*)>> translator;
    std::shared_ptr<plugin_support::PluginInterface> interface;
  };

 public:
  explicit TranslationMaster(
      std::shared_ptr<const ApplicationSettings> settings,
      const std::string& folder,
      std::shared_ptr<const plugin_support::PluginsContainer> plugins,
      QObject* parent = nullptr);

 public slots:
  void onSettingsChanged();
 signals:
  void langChanged();

 private:
  std::shared_ptr<const ApplicationSettings> mSettings;
  Language mLang;
  std::string mFolder;
  QTranslator mTranslator;
  std::shared_ptr<const plugin_support::PluginsContainer> mPlugins;
  std::list<TranslateUnit> mPluginTranslators;
};

#endif  // TRANSLATIONMASTER_H
