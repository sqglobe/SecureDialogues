#ifndef TRANSLATIONMASTER_H
#define TRANSLATIONMASTER_H

#include <QObject>
#include <QTranslator>
#include <memory>
#include <string>
#include "languages.h"

class ApplicationSettings;

class TranslationMaster : public QObject {
  Q_OBJECT
 public:
  explicit TranslationMaster(
      std::shared_ptr<const ApplicationSettings> settings,
      const std::string& folder,
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
};

#endif  // TRANSLATIONMASTER_H
