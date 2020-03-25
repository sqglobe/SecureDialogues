#include "smtpconnectionwidget.h"
#include <QRegExp>
#include "smtpconnectioninfo.h"
#include "ui_smtpconnectionwidget.h"
SmtpConnectionWidget::SmtpConnectionWidget(QWidget* parent) :
    PluginWidget(parent), ui(new Ui::SmtpConnectionWidget) {
  ui->setupUi(this);
}

SmtpConnectionWidget::~SmtpConnectionWidget() {
  delete ui;
}

void SmtpConnectionWidget::setConnectionInfo(
    PluginConnectionInfo* connInfo) noexcept {
  if (auto* conn = dynamic_cast<SmtpConnectionInfo*>(connInfo);
      conn != nullptr) {
    findChild<QLineEdit*>("emailLogin")->setText(conn->userName.c_str());
    findChild<QLineEdit*>("emailFrom")->setText(conn->from.c_str());
    findChild<QLineEdit*>("smtpUrl")->setText(conn->smtpAddr.c_str());
    findChild<QLineEdit*>("imapUrl")->setText(conn->imapAddr.c_str());
    findChild<QLineEdit*>("smtpPort")->setText(QString::number(conn->smtpPort));
    findChild<QLineEdit*>("imapPort")->setText(QString::number(conn->imapPort));

    findChild<QCheckBox*>("useTls")->setChecked(conn->tlsUsed);
  }
}

const char* SmtpConnectionWidget::fillConnectionInfo(
    PluginConnectionInfo* conninfo) noexcept {
  const static QRegExp re(
      "^(\\S+)@([a-z0-9-]+)(\\.)([a-z]{2,4})(\\.?)([a-z]{0,4})+$");
  if (findChild<QLineEdit*>("emailLogin")->text().trimmed().isEmpty()) {
    return tr("Field 'Login' should be completed").toUtf8().data();
  }

  if (findChild<QLineEdit*>("emailPass")->text().trimmed().isEmpty()) {
    return tr("Field 'Password' should be completed").toUtf8().data();
  }

  if (auto text = findChild<QLineEdit*>("emailFrom")->text().trimmed();
      text.isEmpty() || re.indexIn(text) == -1) {
    return tr("Field 'From' empty or contains wrong format").toUtf8().data();
  }

  if (findChild<QLineEdit*>("smtpUrl")->text().trimmed().isEmpty()) {
    return tr("'Address of SMTP server' empty").toUtf8().data();
  }

  {
    auto port = findChild<QLineEdit*>("smtpPort")->text().trimmed();
    bool ok = false;
    if (port.toInt(&ok); port.isEmpty() || !ok) {
      return tr("Field 'Port of SMTP server' empty or contains wrong format")
          .toUtf8()
          .data();
    }
  }

  if (findChild<QLineEdit*>("imapUrl")->text().trimmed().isEmpty()) {
    return tr("'Address of IMAP server' empty").toUtf8().data();
  }

  {
    auto port = findChild<QLineEdit*>("imapPort")->text().trimmed();
    bool ok = false;
    if (port.toInt(&ok); !ok || port.isEmpty()) {
      return tr("'Port of IMAP server' empty or contains wrong format")
          .toUtf8()
          .data();
    }
  }

  if (auto* smtpConn = dynamic_cast<SmtpConnectionInfo*>(conninfo);
      smtpConn != nullptr) {
    smtpConn->userName =
        findChild<QLineEdit*>("emailLogin")->text().trimmed().toStdString();
    smtpConn->password =
        findChild<QLineEdit*>("emailPass")->text().trimmed().toStdString();
    smtpConn->tlsUsed = findChild<QCheckBox*>("useTls")->isChecked();
    smtpConn->from =
        findChild<QLineEdit*>("emailFrom")->text().trimmed().toStdString();
    smtpConn->smtpAddr =
        findChild<QLineEdit*>("smtpUrl")->text().trimmed().toStdString();
    smtpConn->smtpPort =
        findChild<QLineEdit*>("smtpPort")->text().trimmed().toInt();
    smtpConn->imapAddr =
        findChild<QLineEdit*>("imapUrl")->text().trimmed().toStdString();
    smtpConn->imapPort =
        findChild<QLineEdit*>("imapPort")->text().trimmed().toInt();
  }

  return nullptr;
}

void SmtpConnectionWidget::cleareWidget() noexcept {
  findChild<QLineEdit*>("emailLogin")->clear();
  findChild<QLineEdit*>("emailPass")->clear();
  findChild<QCheckBox*>("useTls")->setCheckState(Qt::Unchecked);
  findChild<QLineEdit*>("emailFrom")->clear();
  findChild<QLineEdit*>("smtpUrl")->clear();
  findChild<QLineEdit*>("smtpPort")->clear();
  findChild<QLineEdit*>("imapUrl")->clear();
  findChild<QLineEdit*>("imapPort")->clear();
}

void SmtpConnectionWidget::makeActivated() noexcept {}
