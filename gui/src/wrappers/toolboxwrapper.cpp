#include "toolboxwrapper.h"

#include <QToolBar>

ToolboxWrapper::ToolboxWrapper(QToolBar* toolBar, QObject* parent) :
    QObject(parent) {
  toolBar->addAction(QIcon(":/icons/link-button.png"),
                     tr("Open connection settings"), this,
                     &ToolboxWrapper::connectionsSettingsTriggered);
  toolBar->addAction(QIcon(":/icons/address-book.png"),
                     tr("Add/edit or remove your contacts"), this,
                     &ToolboxWrapper::contactsSettingsTriggered);
  toolBar->addAction(QIcon(":/icons/old-key.png"), tr("Show my public key"),
                     this, &ToolboxWrapper::publicKeySettingsTriggered);
  toolBar->addAction(QIcon(":/icons/document-with-folded-corner.png"),
                     tr("Show recieved contact information"), this,
                     &ToolboxWrapper::recievedContactsSettingsTriggered);
  toolBar->addSeparator();
  toolBar->addAction(QIcon(":/icons/ship-rudder.png"),
                     tr("Show general application settings"), this,
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
