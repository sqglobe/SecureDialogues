#include "connectioninfowidget.h"
#include "ui_connectioninfowidget.h"

#include <QComboBox>
#include <QLineEdit>
#include <functional>
#include <sstream>
#include <utility>
#include "correctnessinputerror.h"
#include "primitives/connectionholder.h"
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
}

ConnectionInfoWidget::~ConnectionInfoWidget() {
  delete ui;
}

void ConnectionInfoWidget::setElement(const ConnectionHolder& info) {
  auto conn_name = findChild<QLineEdit*>("connectionName");
  auto login = findChild<QLineEdit*>("login");

  conn_name->setText(info.getConnectionName().c_str());

  if (info.getType() == ConnectionType::GMAIL) {
    auto gmailConn = info.getConnection<GmailConnaection>();
    login->setText(gmailConn.email.c_str());
  }

  auto connType = findChild<QComboBox*>("connectionType");
  connType->setCurrentIndex(static_cast<int>(info.getType()));

  mInfo.reset(new ConnectionHolder(info));

  activatedConnectionType(static_cast<int>(info.getType()));

  auto connStatus = findChild<QLabel*>("statusText");
  auto lastMessage = findChild<QLabel*>("lastMessage");
  QPalette palette = connStatus->palette();
  switch (info.getStatus()) {
    case Channel::ChannelStatus::UNDEFINED: {
      connStatus->setText("Подключение не подтверждено");
      lastMessage->setText("");
      palette.setColor(connStatus->backgroundRole(), QColor(192, 192, 192));
      lastMessage->hide();
      break;
    }
    case Channel::ChannelStatus::CONNECTED: {
      connStatus->setText("Подключение выполнено успешно");
      palette.setColor(connStatus->backgroundRole(), QColor(152, 251, 152));
      lastMessage->setText("");
      lastMessage->hide();

      break;
    }
    case Channel::ChannelStatus::FAILED_CONNECT: {
      connStatus->setText("Подключение не успешно");
      lastMessage->setText(info.getMessage().c_str());
      palette.setColor(connStatus->backgroundRole(), QColor(255, 192, 203));
      lastMessage->show();
      break;
    }
    case Channel::ChannelStatus::AUTHORIZATION_FAILED: {
      connStatus->setText("Необходимо выполнить авторизацию");
      lastMessage->setText(info.getMessage().c_str());
      palette.setColor(connStatus->backgroundRole(), QColor(250, 128, 114));
      lastMessage->show();
    }
  }
  connStatus->setAutoFillBackground(true);
  connStatus->setPalette(palette);

  findChild<QWidget*>("satusWidget")->show();
}

ConnectionHolder ConnectionInfoWidget::getElement() noexcept(false) {
  std::vector<std::pair<std::string, std::function<bool(void)> > > checks;

  auto connType = findChild<QComboBox*>("connectionType");

  checks.emplace_back("Необходимо выбрать тип подключения!",
                      [connType]() -> bool {
                        return connType->currentIndex() !=
                               static_cast<int>(ConnectionType::UNDEF);
                      });

  if (connType->currentIndex() == static_cast<int>(ConnectionType::GMAIL)) {
    checks.emplace_back("Необходимо заполнить поле 'Логин'", [this]() -> bool {
      auto login = findChild<QLineEdit*>("login");
      return !login->text().isEmpty();
    });

    checks.emplace_back("Необходимо заполнить поле 'Пароль'", [this]() -> bool {
      auto pass = findChild<QLineEdit*>("pass");
      return !pass->text().isEmpty();
    });
  } else if (connType->currentIndex() == static_cast<int>(ConnectionType::VK)) {
    checks.emplace_back("Необходимо заполнить поле 'URL'", [this]() -> bool {
      auto login = findChild<QLineEdit*>("vkOauthUrl");
      return !login->text().isEmpty();
    });
    checks.emplace_back("Значение в поле 'URL' имеет не верный формат",
                        [this]() -> bool {
                          auto txt = findChild<QLineEdit*>("vkOauthUrl");
                          auto url = QUrl(txt->text());
                          if (!url.isValid() || !url.hasFragment())
                            return false;
                          auto query = QUrlQuery(url.fragment());
                          return query.hasQueryItem("access_token") &&
                                 query.hasQueryItem("user_id");
                        });
  }

  auto conn_name = findChild<QLineEdit*>("connectionName");
  checks.emplace_back(
      "Необходимо заполнить поле 'Имя подключения'", [conn_name]() -> bool {
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
        GmailConnaection{login->text().toStdString(), passText});
  } else if (connectionType == ConnectionType::VK) {
    auto url = findChild<QLineEdit*>("vkOauthUrl");
    auto query = QUrlQuery(QUrl(url->text()).fragment());
    connHolder.setConnection(
        VkConnection{query.queryItemValue("user_id").toStdString(),
                     query.queryItemValue("access_token").toStdString()});
  } else {
    connHolder.setConnection(UdpConnection{});
  }

  mInfo.release();
  return connHolder;
}

void ConnectionInfoWidget::actionCleare() {
  emit cleareVal();
  auto connType = findChild<QComboBox*>("connectionType");
  connType->setCurrentIndex(1);
  mInfo.release();
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
    QString href = "<a href='%1'>Открыть страницу получения Oauth2 кода</a>";
    tokenUrl->setText(href.arg(oauth.c_str()));
  } else if (index == static_cast<int>(ConnectionType::VK)) {
    auto tokenUrl = findChild<QLabel*>("vkTokenUrl");
    auto oauth = makeFactory(AgentType::VK)->makeOauthAgent()->getUserUrl();
    QString href = "<a href='%1'>Открыть страницу получения Oauth2 кода</a>";
    tokenUrl->setText(href.arg(oauth.c_str()));
  }
}
