#include "connectioninfowidget.h"
#include "ui_connectioninfowidget.h"

#include <QComboBox>
#include <QLineEdit>
#include <functional>
#include <sstream>
#include <utility>
#include "correctnessinputerror.h"

#include "widgetsutils.h"

#include "oauth-agents/agents/gmailoauth.h"
#include "oauth-agents/utils/factories.h"

#include <QUrlQuery>

ConnectionInfoWidget::ConnectionInfoWidget(QWidget* parent) :
    QWidget(parent), ui(new Ui::ConnectionInfoWidget) {
  ui->setupUi(this);

  auto conn_name = findChild<QLineEdit*>("connectionName");
  auto login = findChild<QLineEdit*>("login");
  auto pass = findChild<QLineEdit*>("pass");
  auto connType = findChild<QComboBox*>("connectionType");
  auto vkOauthUrl = findChild<QLineEdit*>("vkOauthUrl");

  auto tokenUrl = findChild<QLabel*>("tokenUrl");
  tokenUrl->setTextFormat(Qt::RichText);
  tokenUrl->setTextInteractionFlags(Qt::TextBrowserInteraction);
  tokenUrl->setOpenExternalLinks(true);

  auto vkTokenUrl = findChild<QLabel*>("vkTokenUrl");
  vkTokenUrl->setTextInteractionFlags(Qt::TextBrowserInteraction);
  vkTokenUrl->setOpenExternalLinks(true);

  connect(this, &ConnectionInfoWidget::cleareVal, conn_name, &QLineEdit::clear);
  connect(this, &ConnectionInfoWidget::cleareVal, login, &QLineEdit::clear);
  connect(this, &ConnectionInfoWidget::cleareVal, pass, &QLineEdit::clear);
  connect(this, &ConnectionInfoWidget::cleareVal, vkOauthUrl,
          &QLineEdit::clear);

  connect(this, &ConnectionInfoWidget::changeEnabled, conn_name,
          &QLineEdit::setEnabled);
  connect(this, &ConnectionInfoWidget::changeEnabled, login,
          &QLineEdit::setEnabled);
  connect(this, &ConnectionInfoWidget::changeEnabled, pass,
          &QLineEdit::setEnabled);
  connect(this, &ConnectionInfoWidget::changeEnabled, vkOauthUrl,
          &QLineEdit::setEnabled);
  connect(this, &ConnectionInfoWidget::changeEnabled, connType,
          &QComboBox::setEnabled);

  connect(connType, SIGNAL(currentIndexChanged(int)), this,
          SLOT(activatedConnectionType(int)));
  connType->setCurrentIndex(0);

  findChild<QWidget*>("satusWidget")->hide();

  // email connection

  connect(this, &ConnectionInfoWidget::cleareVal,
          findChild<QLineEdit*>("emailLogin"), &QLineEdit::clear);
  connect(this, &ConnectionInfoWidget::changeEnabled,
          findChild<QLineEdit*>("emailLogin"), &QLineEdit::setEnabled);

  connect(this, &ConnectionInfoWidget::cleareVal,
          findChild<QLineEdit*>("emailPass"), &QLineEdit::clear);
  connect(this, &ConnectionInfoWidget::changeEnabled,
          findChild<QLineEdit*>("emailPass"), &QLineEdit::setEnabled);

  connect(this, &ConnectionInfoWidget::cleareVal,
          findChild<QLineEdit*>("emailFrom"), &QLineEdit::clear);
  connect(this, &ConnectionInfoWidget::changeEnabled,
          findChild<QLineEdit*>("emailFrom"), &QLineEdit::setEnabled);

  connect(this, &ConnectionInfoWidget::cleareVal,
          findChild<QLineEdit*>("smtpUrl"), &QLineEdit::clear);
  connect(this, &ConnectionInfoWidget::changeEnabled,
          findChild<QLineEdit*>("smtpUrl"), &QLineEdit::setEnabled);

  connect(this, &ConnectionInfoWidget::cleareVal,
          findChild<QLineEdit*>("imapUrl"), &QLineEdit::clear);
  connect(this, &ConnectionInfoWidget::changeEnabled,
          findChild<QLineEdit*>("imapUrl"), &QLineEdit::setEnabled);

  connect(this, &ConnectionInfoWidget::cleareVal,
          findChild<QLineEdit*>("smtpPort"), &QLineEdit::clear);
  connect(this, &ConnectionInfoWidget::changeEnabled,
          findChild<QLineEdit*>("smtpPort"), &QLineEdit::setEnabled);

  connect(this, &ConnectionInfoWidget::cleareVal,
          findChild<QLineEdit*>("imapPort"), &QLineEdit::clear);
  connect(this, &ConnectionInfoWidget::changeEnabled,
          findChild<QLineEdit*>("imapPort"), &QLineEdit::setEnabled);

  connect(this, &ConnectionInfoWidget::changeEnabled,
          findChild<QCheckBox*>("useTls"), &QCheckBox::setEnabled);
}

ConnectionInfoWidget::~ConnectionInfoWidget() {
  delete ui;
}

ConnectionInfoWidget::Element ConnectionInfoWidget::getChannelInfo(
    const std::string& channelName) const {
  auto it = mCachedElements.find(channelName);
  if (it == mCachedElements.cend()) {
    return {Channel::ChannelStatus::UNDEFINED, ""};
  }
  return it->second;
}

void ConnectionInfoWidget::displayStatus(
    const ConnectionInfoWidget::Element& element) {
  auto connStatus = findChild<QLabel*>("statusText");
  auto lastMessage = findChild<QLabel*>("lastMessage");
  QPalette palette = connStatus->palette();

  switch (element.status) {
    case Channel::ChannelStatus::UNDEFINED: {
      connStatus->setText(tr("Connection not confirmed"));
      lastMessage->setText("");
      palette.setColor(connStatus->backgroundRole(), QColor(192, 192, 192));
      lastMessage->hide();
      break;
    }
    case Channel::ChannelStatus::CONNECTED: {
      connStatus->setText("Connection established successed");
      palette.setColor(connStatus->backgroundRole(), QColor(152, 251, 152));
      lastMessage->setText("");
      lastMessage->hide();

      break;
    }
    case Channel::ChannelStatus::FAILED_CONNECT: {
      connStatus->setText("Connection failed");
      lastMessage->setText(element.message);
      palette.setColor(connStatus->backgroundRole(), QColor(255, 192, 203));
      lastMessage->show();
      break;
    }
    case Channel::ChannelStatus::AUTHORIZATION_FAILED: {
      connStatus->setText("Authorisation required");
      lastMessage->setText(element.message);
      palette.setColor(connStatus->backgroundRole(), QColor(250, 128, 114));
      lastMessage->show();
    }
  }
  connStatus->setAutoFillBackground(true);
  connStatus->setPalette(palette);
}

void ConnectionInfoWidget::setElement(const ConnectionHolder& info) {
  auto conn_name = findChild<QLineEdit*>("connectionName");
  auto login = findChild<QLineEdit*>("login");

  conn_name->setText(info.getConnectionName().c_str());

  if (info.getType() == ConnectionType::GMAIL) {
    auto gmailConn = info.getConnection<GmailConnection>();
    login->setText(gmailConn.email.c_str());
  } else if (info.getType() == ConnectionType::EMAIL) {
    auto connInfo = info.getConnection<EmailConnection>();
    findChild<QLineEdit*>("emailLogin")->setText(connInfo.userName.c_str());
    findChild<QLineEdit*>("emailFrom")->setText(connInfo.from.c_str());
    findChild<QLineEdit*>("smtpUrl")->setText(connInfo.smtpAddr.c_str());
    findChild<QLineEdit*>("imapUrl")->setText(connInfo.imapAddr.c_str());
    findChild<QLineEdit*>("smtpPort")
        ->setText(QString::number(connInfo.smtpPort));
    findChild<QLineEdit*>("imapPort")
        ->setText(QString::number(connInfo.imapPort));

    findChild<QCheckBox*>("useTls")->setChecked(connInfo.tlsUsed);
  }

  auto connType = findChild<QComboBox*>("connectionType");
  connType->setCurrentIndex(static_cast<int>(info.getType()));

  auto statusInfo = getChannelInfo(info.getConnectionName());
  displayStatus(statusInfo);

  mInfo = info;

  activatedConnectionType(static_cast<int>(info.getType()));
  findChild<QWidget*>("satusWidget")->show();
}

ConnectionHolder ConnectionInfoWidget::getElement() noexcept(false) {
  std::vector<std::pair<QString, std::function<bool(void)> > > checks;

  auto connType = findChild<QComboBox*>("connectionType");

  checks.emplace_back(tr("Connection type required!"), [connType]() -> bool {
    return connType->currentIndex() != static_cast<int>(ConnectionType::UNDEF);
  });

  if (connType->currentIndex() == static_cast<int>(ConnectionType::GMAIL)) {
    checks.emplace_back(tr("Field 'Login' should be completed"),
                        [this]() -> bool {
                          auto login = findChild<QLineEdit*>("login");
                          return !login->text().trimmed().isEmpty();
                        });

    checks.emplace_back(tr("Field 'Password' should be completed"),
                        [this]() -> bool {
                          auto pass = findChild<QLineEdit*>("pass");
                          return !pass->text().trimmed().isEmpty();
                        });
  } else if (connType->currentIndex() == static_cast<int>(ConnectionType::VK)) {
    checks.emplace_back(tr("Field 'URL' should be completed"),
                        [this]() -> bool {
                          auto login = findChild<QLineEdit*>("vkOauthUrl");
                          return !login->text().trimmed().isEmpty();
                        });
    checks.emplace_back(tr("Field 'URL' contains wrond format"),
                        [this]() -> bool {
                          auto txt = findChild<QLineEdit*>("vkOauthUrl");
                          auto url = QUrl(txt->text().trimmed());
                          if (!url.isValid() || !url.hasFragment())
                            return false;
                          auto query = QUrlQuery(url.fragment());
                          return query.hasQueryItem("access_token") &&
                                 query.hasQueryItem("user_id");
                        });
  } else if (connType->currentIndex() ==
             static_cast<int>(ConnectionType::EMAIL)) {
    checks.emplace_back(tr("Field 'Login' should be completed"),
                        [this]() -> bool {
                          return !findChild<QLineEdit*>("emailLogin")
                                      ->text()
                                      .trimmed()
                                      .isEmpty();
                        });

    checks.emplace_back(tr("Field 'Password' should be completed"),
                        [this]() -> bool {
                          return !findChild<QLineEdit*>("emailPass")
                                      ->text()
                                      .trimmed()
                                      .isEmpty();
                        });

    checks.emplace_back(
        tr("Field 'From' empty or contains wrong format"), [this]() -> bool {
          QRegExp re(
              "^(\\S+)@([a-z0-9-]+)(\\.)([a-z]{2,4})(\\.?)([a-z]{0,4})+$");
          auto text = findChild<QLineEdit*>("emailFrom")->text().trimmed();
          return !text.isEmpty() && re.indexIn(text) != -1;
        });

    checks.emplace_back(tr("'Address of SMTP server' empty"), [this]() -> bool {
      return !findChild<QLineEdit*>("smtpUrl")->text().trimmed().isEmpty();
    });

    checks.emplace_back(
        tr("Field 'Port of SMTP server' empty or contains wrong format"),
        [this]() -> bool {
          auto port = findChild<QLineEdit*>("smtpPort")->text().trimmed();
          bool ok = false;
          port.toInt(&ok);
          return !port.isEmpty() && ok;
        });

    checks.emplace_back("'Address of IMAP server' empty", [this]() -> bool {
      return !findChild<QLineEdit*>("imapUrl")->text().trimmed().isEmpty();
    });

    checks.emplace_back(
        "'Port of IMAP server' empty or contains wrong format",
        [this]() -> bool {
          auto port = findChild<QLineEdit*>("imapPort")->text().trimmed();
          bool ok = false;
          port.toInt(&ok);
          return !port.isEmpty() && ok;
        });
  }

  auto conn_name = findChild<QLineEdit*>("connectionName");
  checks.emplace_back(
      "Field 'Connection name' should be completed", [conn_name]() -> bool {
        return !conn_name->isEnabled() || !conn_name->text().isEmpty();
      });
  WigetUtils::test(checks);

  auto connectionType = static_cast<ConnectionType>(connType->currentIndex());

  auto connHolder = ConnectionHolder(mInfo ? mInfo->getConnectionName()
                                           : conn_name->text().toStdString());

  if (connectionType == ConnectionType::GMAIL) {
    auto login = findChild<QLineEdit*>("login");
    auto pass = findChild<QLineEdit*>("pass");
    auto passText = pass->text().toStdString();
    auto oauth = makeFactory(AgentType::GMAIL)->makeOauthAgent(passText);
    oauth->loadAccessToken(passText);
    passText = oauth->getRefreshToken();
    connHolder.setConnection(
        GmailConnection{login->text().toStdString(), passText});
  } else if (connectionType == ConnectionType::VK) {
    auto url = findChild<QLineEdit*>("vkOauthUrl");
    auto query = QUrlQuery(QUrl(url->text()).fragment());
    connHolder.setConnection(
        VkConnection{query.queryItemValue("user_id").toStdString(),
                     query.queryItemValue("access_token").toStdString()});
  } else if (connectionType == ConnectionType::EMAIL) {
    auto conn = EmailConnection{
        findChild<QLineEdit*>("emailLogin")->text().trimmed().toStdString(),
        findChild<QLineEdit*>("emailPass")->text().trimmed().toStdString(),
        findChild<QCheckBox*>("useTls")->isChecked(),
        findChild<QLineEdit*>("emailFrom")->text().trimmed().toStdString(),
        findChild<QLineEdit*>("smtpUrl")->text().trimmed().toStdString(),
        findChild<QLineEdit*>("smtpPort")->text().trimmed().toInt(),
        findChild<QLineEdit*>("imapUrl")->text().trimmed().toStdString(),
        findChild<QLineEdit*>("imapPort")->text().trimmed().toInt(),
    };
    connHolder.setConnection(conn);
  } else {
    connHolder.setConnection(UdpConnection{});
  }
  mInfo.reset();
  return connHolder;
}

void ConnectionInfoWidget::updateChannelStatus(Channel::ChannelStatus status,
                                               const std::string& channelName,
                                               const std::string& message) {
  auto it = mCachedElements.find(channelName);
  Element elem{status, QString(message.c_str())};
  if (it == mCachedElements.end()) {
    mCachedElements.emplace(channelName, elem);
  } else {
    it->second = elem;
  }
  if (mInfo && mInfo->getConnectionName() == channelName) {
    displayStatus(elem);
  }
}

void ConnectionInfoWidget::actionCleare() {
  emit cleareVal();
  auto connType = findChild<QComboBox*>("connectionType");
  connType->setCurrentIndex(1);
  mInfo.reset();
  findChild<QWidget*>("satusWidget")->hide();
}

void ConnectionInfoWidget::actionEnable() {
  emit changeEnabled(true);
  if (mInfo) {
    auto conn_name = findChild<QLineEdit*>("connectionName");
    conn_name->setEnabled(false);
  }
}

void ConnectionInfoWidget::actionDisable() {
  emit changeEnabled(false);
}

void ConnectionInfoWidget::activatedConnectionType(int index) {
  if (index == static_cast<int>(ConnectionType::GMAIL)) {
    auto tokenUrl = findChild<QLabel*>("tokenUrl");
    auto oauth = makeFactory(AgentType::GMAIL)->makeOauthAgent()->getUserUrl();
    QString href = tr("<a href='%1'>Open web-page to get Oauth2 code</a>");
    tokenUrl->setText(href.arg(oauth.c_str()));
  } else if (index == static_cast<int>(ConnectionType::VK)) {
    auto tokenUrl = findChild<QLabel*>("vkTokenUrl");
    auto oauth = makeFactory(AgentType::VK)->makeOauthAgent()->getUserUrl();
    QString href = tr("<a href='%1'>Open web-page to get Oauth2 code</a>");
    tokenUrl->setText(href.arg(oauth.c_str()));
  }
}
