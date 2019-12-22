#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QSettings>
#include <memory>

class ApplicationSettings
{
public:
    ApplicationSettings();
public:
    QString getLocale() const;

public:
    void setLocale(const QString &locale);

private:
    QSettings mSettings;
};

class ApplicationSettingsGuard
{
    friend class AppSettingsDialog;

public:
    ApplicationSettingsGuard():mSettings(std::make_shared<ApplicationSettings>()){}

public:
    std::shared_ptr<const ApplicationSettings> getSettings() const{
        return mSettings;
    }

private:
    std::shared_ptr<ApplicationSettings> getEditableSettings() const{
        return mSettings;
    }

private:
    std::shared_ptr<ApplicationSettings> mSettings;
};

#endif // APPLICATIONSETTINGS_H
