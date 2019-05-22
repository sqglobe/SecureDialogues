#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "initializer/coreinitializer.h"
#include "initializer/guiinitializer.h"

#include <QInputDialog>
#include "dialogs/userinformator.h"
#include "wrappers/dialoguserviewwrapper.h"

enum MESSAGE_SWITCH_PAGES { OK_DIALOG_PAGE = 0, BAD_DIALOG_PAGE = 1 };

MainWindow::MainWindow(const std::string& pass, QWidget* parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),
    mUserInformator(std::make_shared<UserInformator>(this)) {
  ui->setupUi(this);

  mCore = std::make_shared<CoreInitializer>(mUserInformator, pass);

  mGui = std::make_shared<GuiInitializer>(this, mCore, mUserInformator,
                                          mUserInformator);
  on_badDialogSelected(
      "Для начала отправки сообщений выберите один диалог из списка");
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::on_commandLinkButton_clicked() {
  auto text = ui->senderText->document()->toPlainText().toStdString();
  if (!text.empty()) {
    emit sendMessage(text);
  }
  ui->senderText->document()->clear();
}

void MainWindow::on_badDialogSelected(std::string) {
  ui->messageContainerSwitch->setCurrentIndex(OK_DIALOG_PAGE);
  ui->commandLinkButton->setDisabled(true);
}

void MainWindow::on_goodDialogSelected(std::string) {
  ui->messageContainerSwitch->setCurrentIndex(OK_DIALOG_PAGE);
  ui->commandLinkButton->setDisabled(false);
}

void MainWindow::closeEvent(QCloseEvent* event) {
  mCore->saveFiles();
  emit closed();
  QMainWindow::closeEvent(event);
}
