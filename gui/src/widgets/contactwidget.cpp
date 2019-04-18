#include "contactwidget.h"
#include "ui_contactwidget.h"

#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>

#include "models/channelslistmodel.h"
#include "primitives/contact.h"
#include "widgetsutils.h"

ContactWidget::ContactWidget(const std::shared_ptr<ChannelsListModel>& model,
                             QWidget* parent) :
    QWidget(parent),
    ui(new Ui::ContactWidget), mModel(model) {
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
}

ContactWidget::~ContactWidget() {
  delete ui;
}

void ContactWidget::setElement(const std::shared_ptr<const Contact>& info) {
  auto userAdress = findChild<QLineEdit*>("contactAdress");
  auto userName = findChild<QLineEdit*>("contactName");
  auto connNames = findChild<QComboBox*>("channelMonikers");
  auto pubKey = findChild<QTextEdit*>("publicKey");

  userAdress->setText(info->adress().c_str());
  userName->setText(info->name().c_str());
  connNames->setCurrentText(info->channelMoniker().c_str());
  pubKey->setText(info->publicKey().c_str());

  mContact = info;
}

std::shared_ptr<Contact> ContactWidget::getElement() {
  std::vector<std::pair<std::string, std::function<bool(void)> > > checks;

  auto userAdress = findChild<QLineEdit*>("contactAdress");
  auto userName = findChild<QLineEdit*>("contactName");
  auto connNames = findChild<QComboBox*>("channelMonikers");
  auto pubKey = findChild<QTextEdit*>("publicKey");

  checks.emplace_back(
      "Необходимо заполнить поле 'Адрес'",
      [userAdress]() -> bool { return !userAdress->text().isEmpty(); });

  checks.emplace_back("Необходимо указать имя контакта", [userName]() -> bool {
    return !userName->text().isEmpty();
  });
  checks.emplace_back(
      "Необходимо заполнить публичный ключ для связи с контактом",
      [pubKey]() -> bool { return !pubKey->toPlainText().isEmpty(); });

  WigetUtils::test(checks);

  std::shared_ptr<Contact> res(
      mContact
          ? std::make_shared<Contact>(
                connNames->currentText().toStdString(),
                userName->text().toStdString(),
                userAdress->text().trimmed().toStdString(),
                pubKey->toPlainText().trimmed().toStdString(), mContact->id())
          : std::make_shared<Contact>(
                connNames->currentText().toStdString(),
                userName->text().toStdString(),
                userAdress->text().trimmed().toStdString(),
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
