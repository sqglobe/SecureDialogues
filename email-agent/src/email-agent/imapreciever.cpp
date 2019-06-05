#include "imapreciever.h"
#include "skipcertverifier.h"

#include <iostream>

#include <iterator>
#include <mutex>
#include "vmime/contentTypeField.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/spdlog.h"

static std::shared_ptr<spdlog::logger> LOGGER =
    spdlog::stdout_color_mt("imap-reciever-logger");

ImapReciever::ImapReciever(const std::string& address,
                           int port,
                           bool tlsUsed,
                           const std::string& folder,
                           const std::string& userName,
                           const std::string& pass,
                           const std::string& subject) :
    mFolder(folder),
    mSubject(subject) {
  auto session = vmime::net::session::create();
  session->getProperties()["server.port"] = port;
  session->getProperties()["connection.tls"] = tlsUsed;

  if (tlsUsed) {
    session->getProperties()["store.imaps.server.address"] = address;
  } else {
    session->getProperties()["store.imap.server.address"] = address;
  }

  mStore = session->getStore(tlsUsed ? "imaps" : "imap");

  mStore->setProperty("options.need-authentication", true);
  mStore->setProperty("auth.username", userName);
  mStore->setProperty("auth.password", pass);
  mStore->setCertificateVerifier(vmime::make_shared<SkipCertVerifier>());
}

void ImapReciever::connect() {
  mStore->connect();

  vmime::net::folder::path path;
  path /= vmime::net::folder::path::component(mFolder);
  mImapFolder = mStore->getFolder(path);
  mImapFolder->open(vmime::net::folder::MODE_READ_ONLY);

  if (mImapFolder->getMessageCount() > 0) {
    auto message = mImapFolder->getMessage(mImapFolder->getMessageCount());
    mImapFolder->fetchMessage(message, vmime::net::fetchAttributes::UID);
    mLastMessage = message->getUID();
  } else {
    mLastMessage = vmime::net::message::uid(1);
  }
}

std::list<std::pair<std::string, std::string> >
ImapReciever::recievedMessages() {
  auto allMessages = mImapFolder->getMessages(
      vmime::net::messageSet::byUID(mLastMessage, "*"));
  mImapFolder->fetchMessages(
      allMessages,
      vmime::net::fetchAttributes::ENVELOPE | vmime::net::fetchAttributes::UID);
  std::list<std::pair<std::string, std::string> > res;

  for (auto it = std::next(allMessages.cbegin()); it != allMessages.cend();
       ++it) {
    auto message = *it;
    auto header = message->getHeader();
    auto from = header->From()->getValue<vmime::mailbox>();
    if (header->Subject()->getValue()->generate() == mSubject) {
      std::string body;
      vmime::utility::outputStreamStringAdapter out(body);
      auto messBody = message->getParsedMessage()->getBody();

      messBody->getContents()->extract(out);
      out.flush();

      res.push_back({from->getEmail().generate(), body});
    }
  }

  mLastMessage = (*allMessages.rbegin())->getUID();

  return res;
}
