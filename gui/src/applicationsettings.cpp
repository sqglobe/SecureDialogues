#include "applicationsettings.h"


ApplicationSettings::ApplicationSettings(): mSettings("secure-dialogues", "app")
{

}

QString ApplicationSettings::getLocale() const
{
    auto value = mSettings.value("language");
    if(value.canConvert<QString>())
    {
        return value.value<QString>();
    }
    return "en_EN.utf8";
}

void ApplicationSettings::setLocale(const QString &locale)
{
    mSettings.setValue("language", locale);
    mSettings.sync();
}