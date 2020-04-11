#include "connectioninfowidget.h"
#include "ui_connectioninfowidget.h"

#include <QComboBox>
#include <QLineEdit>
#include <functional>
#include <sstream>
#include <utility>
#include "correctnessinputerror.h"

#include "widgetsutils.h"

#include "export/pluginconnectioninfo.h"
#include "export/pluginwidget.h"
#include "plugininterface.h"
#include "pluginwidgetwrapper.h"

ConnectionInfoWidget::ConnectionInfoWidget(
    std::shared_ptr<const plugin_support::PluginsContainer> container,
    QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ConnectionInfoWidget) {
  ui->setupUi(this);

  auto stackedWidget = findChild<QStackedWidget*>("stackedWidget");
  auto conn_name = findChild<QLineEdit*>("connectionName");
  auto connType = findChild<QComboBox*>("connectionType");

  for (auto iter = container->cbegin(); iter != container->cend(); ++iter) {
    mWidgets.push_back((*iter)->getWidget());
    stackedWidget->addWidget(mWidgets.back()->getWidget());
    connType->addItem((*iter)->getName().c_str());
  }

  connect(this, &ConnectionInfoWidget::cleareVal, conn_name, &QLineEdit::clear);

  connect(this, &ConnectionInfoWidget::changeEnabled, conn_name,
          &QLineEdit::setEnabled);
  connect(this, &ConnectionInfoWidget::changeEnabled, connType,
          &QComboBox::setEnabled);
  connect(this, &ConnectionInfoWidget::changeEnabled, stackedWidget,
          &QStackedWidget::setEnabled);

  connect(connType, SIGNAL(currentIndexChanged(int)), this,
          SLOT(activatedConnectionType(int)));
  connType->setCurrentIndex(0);

  findChild<QWidget*>("satusWidget")->hide();
}

ConnectionInfoWidget::~ConnectionInfoWidget() {
  std::for_each(std::cbegin(mWidgets), std::cend(mWidgets),
                [](const auto& widget) mutable {
                  widget->getWidget()->setParent(nullptr);
                });
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
      break;
    }
    case Channel::ChannelStatus::BAD_CHANNEL: {
      connStatus->setText("Plugin missed");
      lastMessage->setText(element.message);
      palette.setColor(connStatus->backgroundRole(), QColor(229, 128, 255));
      lastMessage->show();
    }
  }
  connStatus->setAutoFillBackground(true);
  connStatus->setPalette(palette);
}

void ConnectionInfoWidget::setElement(const ConnectionHolder& info) {
  auto conn_name = findChild<QLineEdit*>("connectionName");

  conn_name->setText(info.connName().c_str());
  if (info.pluginConnInfo() != nullptr) {
    auto widget = std::find_if(
        std::cbegin(mWidgets), std::cend(mWidgets),
        [id = info.pluginConnInfo()->getPluginId()](
            const auto& widget) -> bool { return id == widget->getId(); });

    if (widget != std::cend(mWidgets)) {
      (*widget)->setConnectionInfo(info.pluginConnInfo());
      auto connType = findChild<QComboBox*>("connectionType");
      connType->setCurrentIndex(
          static_cast<int>(std::distance(std::cbegin(mWidgets), widget)));
      (*widget)->makeActivated();
    }
  }

  auto statusInfo = getChannelInfo(info.connName());
  displayStatus(statusInfo);

  mInfo = info;

  findChild<QWidget*>("satusWidget")->show();
}

ConnectionHolder ConnectionInfoWidget::getElement() noexcept(false) {
  std::vector<std::pair<QString, std::function<bool(void)> > > checks;

  auto connType = findChild<QComboBox*>("connectionType");

  checks.emplace_back(tr("Connection type required!"), [connType]() -> bool {
    return connType->currentIndex() != 0;
  });

  auto conn_name = findChild<QLineEdit*>("connectionName");
  checks.emplace_back(
      "Field 'Connection name' should be completed", [conn_name]() -> bool {
        return !conn_name->isEnabled() || !conn_name->text().isEmpty();
      });
  WigetUtils::test(checks);

  const auto& widget =
      mWidgets.at(static_cast<std::size_t>(connType->currentIndex() - 1));

  auto connHolder = mInfo ? mInfo.value()
                          : ConnectionHolder(conn_name->text().toStdString(),
                                             widget->makeConnection());
  widget->fillConnection(connHolder.pluginConnInfo());

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
  if (mInfo && mInfo->connName() == channelName) {
    displayStatus(elem);
  }
}

void ConnectionInfoWidget::actionCleare() {
  emit cleareVal();
  auto connType = findChild<QComboBox*>("connectionType");
  connType->setCurrentIndex(1);
  mInfo.reset();
  findChild<QWidget*>("satusWidget")->hide();
  std::for_each(std::cbegin(mWidgets), std::cend(mWidgets),
                [](const auto& widget) mutable { widget->cleareWidget(); });
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
  const auto elIndex = static_cast<std::size_t>(index);
  if (mWidgets.size() >= elIndex && elIndex > 0) {
    mWidgets.at(elIndex - 1)->makeActivated();
  }
}
