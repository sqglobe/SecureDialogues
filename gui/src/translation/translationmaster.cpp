#include "translationmaster.h"

#include "applicationsettings.h"

#include <clocale>
#include <libintl.h>

TranslationMaster::TranslationMaster(std::shared_ptr<const ApplicationSettings> settings,
                                     const std::string &folder,
                                     QObject *parent) : QObject(parent),
    mSettings(std::move(settings)),
    mLang(mSettings->getLocale())
{
    bindtextdomain("secure-dialogues", folder.c_str());
    textdomain("secure-dialogues");

    std::setlocale(LC_ALL, mLang.toStdString().c_str());
}

void TranslationMaster::onSettingsChanged()
{
   if(const auto lang = mSettings->getLocale(); lang != mLang){
       std::setlocale(LC_ALL, lang.toStdString().c_str());
       mLang = lang;
       emit langChanged();
   }

}
