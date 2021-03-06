#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

#include "gui-initializer/eventqueueholder.h"
#include "pluginscontainer.h"

class GuiInitializer;
class AbstractCoreInitializer;
class ToolboxWrapper;
class ApplicationSettingsGuard;
class TranslationMaster;

class UserInformator;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(
      std::shared_ptr<ApplicationSettingsGuard> settingsGuard,
      std::shared_ptr<TranslationMaster> translatorMaster,
      std::unique_ptr<AbstractCoreInitializer>&& coreInit,
      std::shared_ptr<const plugin_support::PluginsContainer> plugins,
      QWidget* parent = nullptr);
  ~MainWindow() override;

 public:
  MainWindow(const MainWindow&) = delete;
  MainWindow& operator=(const MainWindow&) = delete;

 public:
  ToolboxWrapper* getToolBoxWrapper() const;

 public slots:
  void on_badDialogSelected(std::string);
  void on_goodDialogSelected(std::string);

 protected:
  void closeEvent(QCloseEvent* event) override;

 private slots:

  void on_commandLinkButton_clicked();

 signals:

  void sendMessage(std::string message);
  void closed();

 private:
  Ui::MainWindow* ui;

  std::shared_ptr<UserInformator> mUserInformator;

  std::shared_ptr<AbstractCoreInitializer> mCore;
  std::shared_ptr<GuiInitializer> mGui;
  EventQueueHolder mEventHolder;
  ToolboxWrapper* mToolboxWrapper;
};

#endif  // MAINWINDOW_H
