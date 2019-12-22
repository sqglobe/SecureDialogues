#ifndef TOOLBOXWRAPPER_H
#define TOOLBOXWRAPPER_H

#include <QObject>

class QToolBar;

class ToolboxWrapper : public QObject {
  Q_OBJECT
 public:
  explicit ToolboxWrapper(QToolBar* toolBar, QObject* parent = nullptr);

 public slots:
  void connectionsSettingsTriggered();
  void contactsSettingsTriggered();
  void publicKeySettingsTriggered();
  void recievedContactsSettingsTriggered();
  void appSettingsTriggered();

 signals:
  void connectionSettingsOpen();
  void contactSettingsOpen();
  void publicKeySettingsOpen();
  void recievedContactsOpen();
  void appSettingsOpen();
 public slots:
};

#endif  // TOOLBOXWRAPPER_H
