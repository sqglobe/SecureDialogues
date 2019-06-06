#include "smtpsender.h"

#include "skipcertverifier.h"

SmtpSender::SmtpSender(const std::string& address,
                       const std::string& from,
                       int port,
                       bool tlsUsed,
                       const std::string& userName,
                       const std::string& pass,
                       const std::string& subject) :
    mFrom(from),
    mSubject(subject)

{
  auto session = vmime::net::session::create();
  session->getProperties()["server.port"] = port;

  session->getProperties()["connection.tls"] = tlsUsed;

  if (tlsUsed) {
    session->getProperties()["transport.smtps.server.address"] = address;
  } else {
    session->getProperties()["transport.smtp.server.address"] = address;
  }

  mTransport = session->getTransport(tlsUsed ? "smtps" : "smtp");
  mTransport->setProperty("options.need-authentication", true);
  mTransport->setProperty("auth.username", userName);
  mTransport->setProperty("auth.password", pass);
  mTransport->setCertificateVerifier(vmime::make_shared<SkipCertVerifier>());
}

void SmtpSender::connect() {
  mTransport->connect();
}

void SmtpSender::send(const std::string& to, const std::string& body) {
  vmime::messageBuilder mb;
  mb.setSubject(vmime::text(mSubject));
  mb.setExpeditor(vmime::mailbox(mFrom));
  mb.getRecipients().appendAddress(vmime::make_shared<vmime::mailbox>(to));
  mb.getTextPart()->setCharset(vmime::charsets::UTF_8);
  mb.getTextPart()->setText(
      vmime::make_shared<vmime::stringContentHandler>(body));
  auto message = mb.construct();
  mTransport->send(message);
}
