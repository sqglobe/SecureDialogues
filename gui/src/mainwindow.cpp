#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gui-initializer/guiinitializer.h"
#include "interfaces/abstractcoreinitializer.h"

#include <QInputDialog>
#include "dialogs/userinformator.h"
#include "wrappers/dialoguserviewwrapper.h"
#include "wrappers/toolboxwrapper.h"
#include "utils/applicationsettings.h"
#include "translation/translationmaster.h"

enum MESSAGE_SWITCH_PAGES { OK_DIALOG_PAGE = 0, BAD_DIALOG_PAGE = 1 };

MainWindow::MainWindow(
        std::shared_ptr<ApplicationSettingsGuard> settingsGuard,
        std::shared_ptr<TranslationMaster> translatorMaster,
        std::unique_ptr<AbstractCoreInitializer>&& coreInit,
        QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mUserInformator(std::make_shared<UserInformator>(this)),
    mCore(std::move(coreInit)) {
  qRegisterMetaType<AbstractUserNotifier::Severity>(
      "AbstractUserNotifier::Severity");
  qRegisterMetaType<QVector<int>>("QVector<int>");
  qRegisterMetaType<std::string>("std::string");
  qRegisterMetaType<Contact>("Contact");

  ui->setupUi(this);

  mToolboxWrapper = new ToolboxWrapper(ui->mainToolBar, this);

  mCore->init(mUserInformator);

  mGui = std::make_shared<GuiInitializer>(this, mCore, mUserInformator,
                                          mUserInformator,
                                          mEventHolder.channelEventQueue(),
                                          std::move(settingsGuard),
                                          std::move(translatorMaster));
  on_badDialogSelected(
      tr("Please, select one dialog from the list for texting").toStdString());
  mCore->startMessagesHandling(mUserInformator,
                               mEventHolder.channelEventQueue());
}

MainWindow::~MainWindow() {
  delete ui;
}

ToolboxWrapper* MainWindow::getToolBoxWrapper() const {
  return mToolboxWrapper;
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
