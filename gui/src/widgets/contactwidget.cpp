#include "contactwidget.h"
#include "ui_contactwidget.h"

#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>

#include "models/channelslistmodel.h"
#include "primitives/contact.h"
#include "widgetsutils.h"

#include "utils/gui_helpers.h"

Q_DECLARE_METATYPE(ChannelsListModel::ListItem);

ContactWidget::ContactWidget(std::shared_ptr<ChannelsListModel> model,
                             QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ContactWidget), mModel(std::move(model)) {
  ui->setupUi(this);

  auto userAdress = findChild<QLineEdit*>("contactAdress");
  auto userName = findChild<QLineEdit*>("contactName");
  auto connNames = findChild<QComboBox*>("channelMonikers");
  auto pubKey = findChild<QTextEdit*>("publicKey");

  connNames->setModel(mModel.get());

  connect(this, &ContactWidget::cleareVal, userAdress, &QLineEdit::clear);
  connect(this, &ContactWidget::cleareVal, userName, &QLineEdit::clear);
  connect(this, &ContactWidget::cleareVal, pubKey, &QTextEdit::clear);

  connect(this, &ContactWidget::changeEnabled, userAdress,
          &QLineEdit::setEnabled);
  connect(this, &ContactWidget::changeEnabled, userName,
          &QLineEdit::setEnabled);
  connect(this, &ContactWidget::changeEnabled, connNames,
          &QComboBox::setEnabled);
  connect(this, &ContactWidget::changeEnabled, pubKey, &QTextEdit::setEnabled);

  connect(connNames, SIGNAL(currentIndexChanged(int)), this,
          SLOT(connectionNameUpdated(int)));
  connectionNameUpdated(0);
}

ContactWidget::~ContactWidget() {
  delete ui;
}

void ContactWidget::setElement(const Contact& info) {
  auto userAdress = findChild<QLineEdit*>("contactAdress");
  auto userName = findChild<QLineEdit*>("contactName");
  auto connNames = findChild<QComboBox*>("channelMonikers");
  auto pubKey = findChild<QTextEdit*>("publicKey");

  userAdress->setText(make_qstring(info.adress()));
  userName->setText(make_qstring(info.name()));
  connNames->setCurrentText(make_qstring(info.channelMoniker()));
  pubKey->setText(make_qstring(info.publicKey()));

  mContact = info;
}

Contact ContactWidget::getElement() {
  std::vector<std::pair<QString, std::function<bool(void)> > > checks;

  auto userAdress = findChild<QLineEdit*>("contactAdress");
  auto userName = findChild<QLineEdit*>("contactName");
  auto connNames = findChild<QComboBox*>("channelMonikers");
  auto pubKey = findChild<QTextEdit*>("publicKey");
  auto connInfoVariant = connNames->currentData();

  auto userAddressText = userAdress->text().trimmed();

  checks.emplace_back(
      tr("Field 'Address' should be completed"),
      [userAdress]() -> bool { return !userAdress->text().isEmpty(); });

  checks.emplace_back("Please, set contact name", [userName]() -> bool {
    return !userName->text().isEmpty();
  });
  checks.emplace_back(
      tr("Please declare public key for commnication with contact"),
      [pubKey]() -> bool { return !pubKey->toPlainText().isEmpty(); });
  if (connInfoVariant.canConvert<ChannelsListModel::ListItem>()) {
    auto item = qvariant_cast<ChannelsListModel::ListItem>(connInfoVariant);
    if (item.connectionType == ConnectionType::VK) {
      checks.emplace_back(
          tr("Field 'Address' has to contains vk id (for example 123451) or reference"
          " to your contact page"),
          [userAddressText, type = item.connectionType]() -> bool {
            return is_address_valid(userAddressText.toUtf8().constData(), type);
          });
    } else if (item.connectionType == ConnectionType::GMAIL ||
               item.connectionType == ConnectionType::EMAIL) {
      checks.emplace_back(
          tr("Please, fill 'Address' with e-mail"),
          [userAddressText, type = item.connectionType]() -> bool {
            return is_address_valid(userAddressText.toUtf8().constData(), type);
          });
    }
  }

  WigetUtils::test(checks);
  if (connInfoVariant.canConvert<ChannelsListModel::ListItem>()) {
    auto item = qvariant_cast<ChannelsListModel::ListItem>(connInfoVariant);
    if (auto re = QRegExp("^\\D+(\\d+)$");
        item.connectionType == ConnectionType::VK &&
        re.indexIn(userAddressText) != -1) {
      userAddressText = re.cap(1);
    }
  }

  Contact res(mContact
                  ? Contact(connNames->currentText().toStdString(),
                            userName->text().trimmed().toStdString(),
                            userAddressText.toStdString(),
                            pubKey->toPlainText().trimmed().toStdString(),
                            std::string(mContact->id()))
                  : Contact(connNames->currentText().toStdString(),
                            userName->text().trimmed().toStdString(),
                            userAddressText.toStdString(),
                            pubKey->toPlainText().trimmed().toStdString()));

  mContact.reset();

  return res;
}

void ContactWidget::actionCleare() {
  emit cleareVal();
  mContact.reset();
}

void ContactWidget::actionEnable() {
  emit changeEnabled(true);
}

void ContactWidget::actionDisable() {
  emit changeEnabled(false);
}

void ContactWidget::connectionNameUpdated(int) {
  auto connNames = findChild<QComboBox*>("channelMonikers");
  auto connInfoVariant = connNames->currentData();
  if (connInfoVariant.canConvert<ChannelsListModel::ListItem>()) {
    auto item = qvariant_cast<ChannelsListModel::ListItem>(connInfoVariant);
    auto userAdress = findChild<QLineEdit*>("contactAdress");
    if (item.connectionType == ConnectionType::VK) {
      userAdress->setPlaceholderText(tr("Reference to VK user page or VK ID"));
      userAdress->setToolTip(
          tr("Specify reference to VK page (example "
          "'https://vk.com/id99900'), or simple VK ID (example id99900 or "
          "99900 )"));
    } else if (item.connectionType == ConnectionType::EMAIL) {
      userAdress->setPlaceholderText(tr("E-mail address"));
      userAdress->setToolTip(tr("Please, set valid e-mail address"));
    } else if (item.connectionType == ConnectionType::GMAIL) {
      userAdress->setPlaceholderText(tr("E-mail address Gmail"));
      userAdress->setToolTip(tr("Please, specify valid e-mail address Gmail service"));
    } else if (item.connectionType == ConnectionType::UDP) {
      userAdress->setPlaceholderText(tr("IP address"));
      userAdress->setToolTip(tr("Set valid IP address or host name"));
    } else {
      userAdress->setPlaceholderText("");
      userAdress->setToolTip("");
    }
  }
}
