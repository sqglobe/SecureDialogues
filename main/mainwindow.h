#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>

class GuiInitializer;
class CoreInitializer;

class UserInformator;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(const std::string& pass, QWidget* parent = nullptr);
  ~MainWindow() override;

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

  std::shared_ptr<CoreInitializer> mCore;
  std::shared_ptr<GuiInitializer> mGui;
};

#endif  // MAINWINDOW_H
