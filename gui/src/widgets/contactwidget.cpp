#include "contactwidget.h"
#include "ui_contactwidget.h"

#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>

#include "models/channelslistmodel.h"
#include "primitives/contact.h"
#include "widgetsutils.h"

#include <memory>
#include "correctnessinputerror.h"
#include "export/pluginaddressvalidator.h"
#include "plugininterface.h"
#include "support-functions.h"
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
  checks.emplace_back(
      tr("Selected connection without appropriative plugin"),
      [connInfoVariant]() -> bool {
        return connInfoVariant.canConvert<ChannelsListModel::ListItem>() &&
               qvariant_cast<ChannelsListModel::ListItem>(connInfoVariant)
                   .validator;
      });

  WigetUtils::test(checks);

  auto item = qvariant_cast<ChannelsListModel::ListItem>(connInfoVariant);
  if (const auto mess = plugin_support::make_string(
          item.validator->isValid(userAddressText.toUtf8().data()));
      !mess.empty()) {
    throw CorrectnessInputError(mess);
  }

  Contact res(mContact
                  ? Contact(connNames->currentText().toStdString(),
                            userName->text().trimmed().toStdString(),
                            std::string(item.validator->peelAddress(
                                userAddressText.toUtf8().data())),
                            pubKey->toPlainText().trimmed().toStdString(),
                            std::string(mContact->id()))
                  : Contact(connNames->currentText().toStdString(),
                            userName->text().trimmed().toStdString(),
                            std::string(item.validator->peelAddress(
                                userAddressText.toUtf8().data())),
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
    if (item.validator) {
      userAdress->setPlaceholderText(item.validator->getPlaceholder());
      userAdress->setToolTip(item.validator->getToolTip());
    } else {
      userAdress->setPlaceholderText("");
      userAdress->setToolTip("");
    }
  }
}
