#include "toolboxwrapper.h"

#include <QToolBar>

ToolboxWrapper::ToolboxWrapper(QToolBar* toolBar, QObject* parent) :
    QObject(parent) {
  toolBar->addAction(QIcon(":/icons/link-button.png"),
                     "Открыть дилог для настройки соединений", this,
                     &ToolboxWrapper::connectionsSettingsTriggered);
  toolBar->addAction(QIcon(":/icons/address-book.png"),
                     "Добавить/редактировать контакты", this,
                     &ToolboxWrapper::contactsSettingsTriggered);
  toolBar->addAction(QIcon(":/icons/old-key.png"),
                     "Показать мой публичный ключ", this,
                     &ToolboxWrapper::publicKeySettingsTriggered);
  toolBar->addAction(QIcon(":/icons/document-with-folded-corner.png"),
                     "Показать полученные контактные данные", this,
                     &ToolboxWrapper::recievedContactsSettingsTriggered);
  toolBar->addSeparator();
  toolBar->addAction(QIcon(":/icons/ship-rudder.png"),
                     "Показать общие настройки приложения", this,
                     &ToolboxWrapper::appSettingsTriggered);
}

void ToolboxWrapper::connectionsSettingsTriggered() {
  emit connectionSettingsOpen();
}

void ToolboxWrapper::contactsSettingsTriggered() {
  emit contactSettingsOpen();
}

void ToolboxWrapper::publicKeySettingsTriggered() {
  emit publicKeySettingsOpen();
}

void ToolboxWrapper::recievedContactsSettingsTriggered() {
  emit recievedContactsOpen();
}

void ToolboxWrapper::appSettingsTriggered() {
  emit appSettingsOpen();
}
