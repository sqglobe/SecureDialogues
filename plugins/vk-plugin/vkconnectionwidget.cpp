#include "vkconnectionwidget.h"
#include "ui_vkconnectionwidget.h"

#include <QUrl>
#include <QUrlQuery>
#include "vk-communication/vkoauth.h"
#include "vkconnectioninfo.h"
const std::string VK_REDIRECT_URI = "https://oauth.vk.com/blank.html";
VkConnectionWidget::VkConnectionWidget(QWidget* parent) :
    PluginWidget(parent), ui(new Ui::VkConnectionWidget) {
  ui->setupUi(this);
}

VkConnectionWidget::~VkConnectionWidget() {
  delete ui;
}

void VkConnectionWidget::setConnectionInfo(PluginConnectionInfo*) noexcept {}

const char* VkConnectionWidget::fillConnectionInfo(
    PluginConnectionInfo* conninfo) noexcept {
  const auto text = findChild<QLineEdit*>("vkOauthUrl")->text().trimmed();
  if (text.isEmpty()) {
    return tr("Field 'URL' should be completed").toUtf8().data();
  }

  auto url = QUrl(text);

  auto query = QUrlQuery(url.fragment());
  if (!url.isValid() || !url.hasFragment() ||
      !query.hasQueryItem("access_token") || !query.hasQueryItem("user_id")) {
    return tr("Field 'URL' contains wrond format").toUtf8().data();
  }
  if (auto* vkConn = dynamic_cast<VkConnectionInfo*>(conninfo);
      vkConn != nullptr) {
    vkConn->vkId = query.queryItemValue("user_id").toStdString();
    vkConn->accessToken = query.queryItemValue("access_token").toStdString();
  }
  return nullptr;
}

void VkConnectionWidget::cleareWidget() noexcept {
  findChild<QLineEdit*>("vkOauthUrl")->clear();
}

void VkConnectionWidget::makeActivated() noexcept {
  auto tokenUrl = findChild<QLabel*>("vkTokenUrl");
  auto oauth = VkOauth(VK_CLIENT_ID, VK_REDIRECT_URI).getUserUrl();
  QString href = tr("<a href='%1'>Open web-page to get Oauth2 code</a>");
  tokenUrl->setText(href.arg(oauth.c_str()));
}
