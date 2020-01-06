#include "gmailconnectionwidget.h"
#include "ui_gmailconnectionwidget.h"

#include "gmailconnectioninfo.h"

#include "oauth-agents/agents/gmailoauth.h"
#include "oauth-agents/utils/factories.h"

GmailConnectionWidget::GmailConnectionWidget(QWidget* parent) :
    PluginWidget(parent), ui(new Ui::GmailConnectionWidget) {
  ui->setupUi(this);
}

GmailConnectionWidget::~GmailConnectionWidget() {
  delete ui;
}

const char* GmailConnectionWidget::getErrorMessage() const noexcept {
  const auto* login = findChild<QLineEdit*>("login");
  const auto* pass = findChild<QLineEdit*>("pass");
  if (login->text().trimmed().isEmpty()) {
    return tr("Field 'Login' should be completed").toUtf8().data();
  } else if (pass->text().trimmed().isEmpty()) {
    return tr("Field 'Token' should be completed").toUtf8().data();
  }

  try {
    auto passText = pass->text().toStdString();
    auto oauth = makeFactory(AgentType::GMAIL)->makeOauthAgent(passText);
    oauth->loadAccessToken(passText);
    passText = oauth->getRefreshToken();

    if (mConnInfo == nullptr) {
      mConnInfo =
          new GmailConnectionInfo{login->text().toStdString(), passText};
    } else {
      mConnInfo->email = login->text().toStdString();
      mConnInfo->accessToken = passText;
    }
  } catch (const std::exception& ex) {
    return ex.what();
  }

  return nullptr;
}

PluginConnectionInfo* GmailConnectionWidget::makeConnectionInfo() noexcept {
  auto* conn = mConnInfo;
  mConnInfo = nullptr;

  return conn;
}

void GmailConnectionWidget::setConnectionInfo(
    PluginConnectionInfo* connInfo) noexcept {
  if (auto* conn = dynamic_cast<GmailConnectionInfo*>(connInfo);
      conn != nullptr) {
    auto* login = findChild<QLineEdit*>("login");
    login->setText(conn->email.c_str());
    mConnInfo = conn;
  }
}

void GmailConnectionWidget::cleareWidget() noexcept {
  findChild<QLineEdit*>("login")->clear();
  findChild<QLineEdit*>("pass")->clear();
}

void GmailConnectionWidget::makeEnable() noexcept {
  findChild<QLineEdit*>("login")->setEnabled(true);
  findChild<QLineEdit*>("pass")->setEnabled(true);
}

void GmailConnectionWidget::makeDisable() noexcept {
  findChild<QLineEdit*>("login")->setEnabled(false);
  findChild<QLineEdit*>("pass")->setEnabled(false);
}

void GmailConnectionWidget::makeActivated() noexcept {
  const auto oauth =
      makeFactory(AgentType::GMAIL)->makeOauthAgent()->getUserUrl();
  QString href = tr("<a href='%1'>Open web-page to get Oauth2 code</a>");
  findChild<QLabel*>("tokenUrl")->setText(href.arg(oauth.c_str()));
}
