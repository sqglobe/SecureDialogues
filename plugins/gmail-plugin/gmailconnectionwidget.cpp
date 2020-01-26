#include "gmailconnectionwidget.h"
#include "ui_gmailconnectionwidget.h"

#include "gmail-communication/gmailoauth.h"
#include "gmailconnectioninfo.h"

#include <memory>

GmailConnectionWidget::GmailConnectionWidget(QWidget* parent) :
    PluginWidget(parent), ui(new Ui::GmailConnectionWidget) {
  ui->setupUi(this);
}

GmailConnectionWidget::~GmailConnectionWidget() {
  delete ui;
}

void GmailConnectionWidget::setConnectionInfo(
    PluginConnectionInfo* connInfo) noexcept {
  if (auto* conn = dynamic_cast<GmailConnectionInfo*>(connInfo);
      conn != nullptr) {
    auto* login = findChild<QLineEdit*>("login");
    login->setText(conn->email.c_str());
  }
}

const char* GmailConnectionWidget::fillConnectionInfo(
    PluginConnectionInfo* conninfo) noexcept {
  const auto* login = findChild<QLineEdit*>("login");
  const auto* pass = findChild<QLineEdit*>("pass");
  if (login->text().trimmed().isEmpty()) {
    return tr("Field 'Login' should be completed").toUtf8().data();
  } else if (pass->text().trimmed().isEmpty()) {
    return tr("Field 'Token' should be completed").toUtf8().data();
  }

  try {
    auto passText = pass->text().toStdString();
    auto oauth = std::make_unique<GmailOauth>(
        GOOGLE_CLIENT_ID, GOOGLE_CLIENT_SECRET, REDIRECT_URL, passText);
    oauth->loadAccessToken(passText);
    passText = oauth->getRefreshToken();

    if (auto* gmailConn = dynamic_cast<GmailConnectionInfo*>(conninfo);
        gmailConn != nullptr) {
      gmailConn->email = login->text().toStdString();
      gmailConn->accessToken = passText;
    }
  } catch (const std::exception& ex) {
    return ex.what();
  }

  return nullptr;
}

void GmailConnectionWidget::cleareWidget() noexcept {
  findChild<QLineEdit*>("login")->clear();
  findChild<QLineEdit*>("pass")->clear();
}

void GmailConnectionWidget::makeActivated() noexcept {
  const auto oauth =
      GmailOauth(GOOGLE_CLIENT_ID, GOOGLE_CLIENT_SECRET, REDIRECT_URL)
          .getUserUrl();
  QString href = tr("<a href='%1'>Open web-page to get Oauth2 code</a>");
  findChild<QLabel*>("tokenUrl")->setText(href.arg(oauth.c_str()));
}
